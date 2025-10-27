// works with MKR1010 , VEML7700 , Touch Sensor
#include <Adafruit_VEML7700.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <utility/wifi_drv.h>  // library to drive to RGB LED on the MKR1010

#define TOUCH_PIN 6 // define the pin of the touch sensor

Adafruit_VEML7700 veml = Adafruit_VEML7700();

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* ssid1 = SECRET_SSID1;
const char* password1 = SECRET_PASS1;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;

// create wifi object and mqtt object
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Light ID & topic
String lightId = "22";  
String mqtt_topic = "student/CASA0014/luminaire/" + lightId;
String clientId = "Tong";  

const int num_leds = 72;
const int payload_size = num_leds * 3;  
byte RGBpayload[payload_size];

// Party mode
float hue = 0.0f;  
unsigned long lastAnimMs = 0;
const unsigned long animStepMs = 30;

// The mode controled by touch sensor
enum Mode { MODE_SENSOR = 0,
            MODE_LAMP = 1,
            MODE_PARTY = 2 };
Mode mode = MODE_SENSOR;

bool lastTouchState = false;
unsigned long lastTouchChange = 0;
const unsigned long touchDebounceMs = 400;

unsigned long lastPublishMs = 0;
const unsigned long publishIntervalMs = 250;

int lastR = -1, lastG = -1, lastB = -1;



void setup() {
  pinMode(TOUCH_PIN, INPUT);

  Serial.begin(115200);
  Serial.println("Vespera with VEML7700 Light Sensor");

  // Connect to WiFi
  startWifi();

  // Connect to MQTT broker
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setBufferSize(2000);
  mqttClient.setCallback(callback);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

  Serial.print("This device is Vespera ");
  Serial.println(lightId);
  Serial.println("Set-up complete");

  // initialize VEML7700
  if (!veml.begin()) {
    Serial.println("Error: VEML7700 not found. Check wiring!");
    while (1);
  }
  Serial.println("VEML7700 sensor ready.");
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_100MS);
  Serial.println("VEML7700 configured.");
}

void loop() {
  // Network connection
  if (!mqttClient.connected()) reconnectMQTT();
  if (WiFi.status() != WL_CONNECTED) startWifi();
  mqttClient.loop();

  unsigned long now = millis();

  // touch detection
  bool touchState = digitalRead(TOUCH_PIN);  
  if (touchState != lastTouchState && (now - lastTouchChange) > touchDebounceMs) {
    lastTouchChange = now;
    lastTouchState = touchState;

    if (touchState == HIGH) {
      if (mode == MODE_SENSOR) mode = MODE_LAMP;
      else if (mode == MODE_LAMP) mode = MODE_PARTY;
      else mode = MODE_SENSOR;

      lastR = lastG = lastB = -1;
      Serial.print("Mode -> ");
      Serial.println(mode == MODE_SENSOR ? "SENSOR" : (mode == MODE_LAMP ? "LAMP" : "PARTY"));
    }
  }

  int r = 0, g = 0, b = 0;

  // Party mode
  if (mode == MODE_PARTY) {
    // random shining
    if (now - lastAnimMs >= animStepMs) {
      lastAnimMs = now;
      for (int pixel = 0; pixel < num_leds; pixel++) {
        if (random(0, 100) < 50) {
          RGBpayload[pixel * 3 + 0] = random(50, 256);
          RGBpayload[pixel * 3 + 1] = random(50, 256);
          RGBpayload[pixel * 3 + 2] = random(50, 256);
        } else {
          RGBpayload[pixel * 3 + 0] *= 0.8;
          RGBpayload[pixel * 3 + 1] *= 0.8;
          RGBpayload[pixel * 3 + 2] *= 0.8;
        }
      }
      if (mqttClient.connected()) {
        mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
      }
    }
// Nomal Lamp mode
  } else if (mode == MODE_LAMP) {
    // Lamp colour
    r = 255;
    g = 255;
    b = 240;
    for (int pixel = 0; pixel < num_leds; pixel++) {
      RGBpayload[pixel * 3 + 0] = (byte)r;
      RGBpayload[pixel * 3 + 1] = (byte)g;
      RGBpayload[pixel * 3 + 2] = (byte)b;
    }
    if (mqttClient.connected()) {
      mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    }
  } 
  // Back to light sensor mode 
  else if (mode == MODE_SENSOR) {
  float lux = veml.readLux();
  Serial.print("Lux: ");
  Serial.println(lux);

  // lux volume control colour
  float t = constrain(map(lux, 0, 2000, 0, 1000) / 1000.0, 0.0, 1.0);
  int targetR = 120 + t * (255 - 120);
  int targetG = 0   + t * (220 - 0);
  int targetB = 255 - t * 255;

  // light flow status
  static float wavePhase = 0;
  wavePhase += 0.25;  // Change Speed 0.1~0.5

  // smoothly change
  static float smoothR[72], smoothG[72], smoothB[72];

  // define rgb light
  const int leds_per_row = 12;   
  const int num_rows = 6;        

  for (int pixel = 0; pixel < num_leds; pixel++) {
    int row = pixel / leds_per_row;     // 0 is top row
    float factor = 0.3 + 0.7 * (0.5 + 0.5 * sin(wavePhase - row * 1.2));

    int targetPixelR = constrain(targetR * factor, 0, 255);
    int targetPixelG = constrain(targetG * factor, 0, 255);
    int targetPixelB = constrain(targetB * factor, 0, 255);

    // Smoothly change the value
    smoothR[pixel] += (targetPixelR - smoothR[pixel]) * 0.12;
    smoothG[pixel] += (targetPixelG - smoothG[pixel]) * 0.12;
    smoothB[pixel] += (targetPixelB - smoothB[pixel]) * 0.12;

    RGBpayload[pixel * 3 + 0] = (byte)smoothR[pixel];
    RGBpayload[pixel * 3 + 1] = (byte)smoothG[pixel];
    RGBpayload[pixel * 3 + 2] = (byte)smoothB[pixel];
  }

  if (mqttClient.connected()) {
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
  }

  if (lux < 200) LedBlue(); 
  else LedGreen();
}

  delay(100);
}

//   for (int n = 0; n < num_leds; n++) {
//     send_all_off();
//     delay(100);
//     send_RGB_to_pixel(0, 250, 0, n);
//     delay(200);
//   }

//   delay(1000);
// }

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}