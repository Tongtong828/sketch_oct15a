// works with MKR1010 , VEML7700 , Touch Sensor
#include <Adafruit_VEML7700.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <utility/wifi_drv.h>  

#define TOUCH_PIN 6  // define the pin of the touch sensor

Adafruit_VEML7700 veml = Adafruit_VEML7700();

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Light ID & topic
String lightId = "22";
String mqtt_topic = "student/CASA0014/luminaire/" + lightId;
String clientId = "Tong";

const int num_leds = 72;
const int payload_size = num_leds * 3;
byte RGBpayload[payload_size];

unsigned long lastAnimMs = 0;
const unsigned long animStepMs = 30;

enum Mode { MODE_SENSOR = 0,
            MODE_LAMP = 1,
            MODE_PARTY = 2 };
Mode mode = MODE_SENSOR;

bool lastTouchState = false;
unsigned long lastTouchChange = 0;
const unsigned long touchDebounceMs = 400;



void setup() {
  pinMode(TOUCH_PIN, INPUT);

  Serial.begin(115200);
  Serial.println("Vespera with VEML7700 with Touch Sensor");

  // Connect WiFi
  startWifi();

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setBufferSize(2000);
  mqttClient.setCallback(callback);

  // Initialize VEML7700
  if (!veml.begin()) {
    Serial.println("Error: VEML7700 not found. Check wiring!");
    while (1);
  }
  Serial.println("VEML7700 sensor ready.");
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_100MS);
}

void loop() {
  if (!mqttClient.connected()) reconnectMQTT();
  if (WiFi.status() != WL_CONNECTED) startWifi();
  mqttClient.loop();

  unsigned long now = millis();

  // touch sensor logic
  bool touchState = digitalRead(TOUCH_PIN);  
  if (touchState != lastTouchState && (now - lastTouchChange) > touchDebounceMs) {
    lastTouchChange = now;
    lastTouchState = touchState;

    if (touchState == HIGH) {
      if (mode == MODE_SENSOR) mode = MODE_LAMP;
      else if (mode == MODE_LAMP) mode = MODE_PARTY;
      else mode = MODE_SENSOR;

      Serial.print("Mode -> ");
      Serial.println(mode == MODE_SENSOR ? "SENSOR" : (mode == MODE_LAMP ? "LAMP" : "PARTY"));
    }
  }

  int r = 0, g = 0, b = 0;

  //light change
  if (mode == MODE_PARTY) {
    // Party mode
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

  } else if (mode == MODE_LAMP) {
    // LAMP
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

  } else if (mode == MODE_SENSOR) {
    // VEML7700
    float lux = veml.readLux();
    Serial.print("Lux: ");
    Serial.println(lux);

    float t = constrain(map(lux, 0, 2000, 0, 1000) / 1000.0, 0.0, 1.0);
    int yellowR = 255, yellowG = 200, yellowB = 0;
    int purpleR = 150, purpleG = 0, purpleB = 200;
    r = purpleR + t * (yellowR - purpleR);
    g = purpleG + t * (yellowG - purpleG);
    b = purpleB + t * (yellowB - purpleB);

    for (int pixel = 0; pixel < num_leds; pixel++) {
      RGBpayload[pixel * 3 + 0] = (byte)r;
      RGBpayload[pixel * 3 + 1] = (byte)g;
      RGBpayload[pixel * 3 + 2] = (byte)b;
    }
    if (mqttClient.connected()) {
      mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    }

    // MKR1010 LED
    if (lux < 200) LedBlue();
    else LedGreen();
  }

  delay(100);
}
