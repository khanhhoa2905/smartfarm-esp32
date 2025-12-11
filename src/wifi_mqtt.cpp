#include "wifi_mqtt.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPIFFS.h>
#include "actuators.h"

const char* ssid = "T4";
const char* password = "44444444";
const char* mqtt_server = "rf51c8c2.ala.asia-southeast1.emqxsl.com";
const int mqtt_port = 8883;
const char* mqtt_user = "smartfarm";
const char* mqtt_pass = "12345678";

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastReconnect = 0;

void callback(char* topic, byte* msg, unsigned int len) {
  String message;
  for (int i = 0; i < len; i++) message += (char)msg[i];
  if (String(topic) == "smartfarm/cmd/light") setLED(message=="ON");
  else if (String(topic) == "smartfarm/cmd/fan") setFan(message=="ON");
  else if (String(topic) == "smartfarm/cmd/pump") setPump(message=="ON");
}

void reconnect() {
  if (millis() - lastReconnect < 5000) return;
  lastReconnect = millis();
  if (client.connect("smartfarm", mqtt_user, mqtt_pass)) {
    client.subscribe("smartfarm/cmd/#");
    Serial.println("MQTT connected");
  }
}

void initWifiMQTT() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("Connected!");
  if (!SPIFFS.begin(true)) Serial.println("SPIFFS Fail");

  File ca = SPIFFS.open("/ca.crt", "r");
if (ca) {
  espClient.loadCACert(ca, ca.size());   // thêm size để phù hợp chuẩn mới
  ca.close();
}


  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void mqttLoop() {
  if (!client.connected()) reconnect();
  client.loop();
}

void mqttPublish(const char* topic, String payload) {
  if (client.connected()) client.publish(topic, payload.c_str());
}
