// works with MKR1010 + VEML7700
#include <Adafruit_VEML7700.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <utility/wifi_drv.h>  // drive RGB LED on MKR1010

Adafruit_VEML7700 veml = Adafruit_VEML7700();

// WiFi + MQTT settings
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

unsigned long lastPublishMs = 0;
const unsigned long publishIntervalMs = 300;

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  Serial.println("Vespera - VEML7700 Light Reactive Controller");

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

// ================== MAIN LOOP ==================
void loop() {
  if (!mqttClient.connected()) reconnectMQTT();
  if (WiFi.status() != WL_CONNECTED) startWifi();
  mqttClient.loop();

  float lux = veml.readLux();
  Serial.print("Lux: ");
  Serial.println(lux);

  // lux volume control colour
  float t = constrain(map(lux, 0, 2000, 0, 1000) / 1000.0, 0.0, 1.0);
  int r = 150 + t * (255 - 150);
  int g = 0   + t * (200 - 0);
  int b = 200 - t * 200;

  // sent colour
  for (int pixel = 0; pixel < num_leds; pixel++) {
    RGBpayload[pixel * 3 + 0] = (byte)r;
    RGBpayload[pixel * 3 + 1] = (byte)g;
    RGBpayload[pixel * 3 + 2] = (byte)b;
  }

  // fixed time update
  unsigned long now = millis();
  if (now - lastPublishMs >= publishIntervalMs) {
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    lastPublishMs = now;
  }

  // load MKR1010 status
  if (lux < 200) LedBlue();
  else LedGreen();

  delay(200);
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i > 0) Serial.print(":");
  }
  Serial.println();
}


