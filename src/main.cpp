#include <Arduino.h>
#include <ArduinoJson.h>
#include "wifi_mqtt.h"
#include "sensors.h"
#include "actuators.h"
#include "automation.h"
#include "rfid.h"

unsigned long lastPublish = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n=== SMART FARM SYSTEM STARTING ===");
  initActuators();
  initSensors();
  initRFID();
  initWifiMQTT();

  Serial.println("✅ System Ready!");
}

void loop() {
  mqttLoop();
  SensorData data = readSensors();
  String uid = checkRFID();  // Đọc RFID
  runAutomation(data, uid);

  // Gửi dữ liệu cảm biến mỗi 2s
  if (millis() - lastPublish > 2000) {
    lastPublish = millis();

    StaticJsonDocument<256> doc;
    doc["temperature"] = data.temp;
    doc["humidity"]    = data.hum;
    doc["soil"]        = data.soil;
    doc["gas"]         = data.gas;
    doc["fire"]        = (data.fire == LOW);
    doc["motion"]      = data.motion;
    doc["light"]       = (data.light == LOW);

    float height = 10.0;
    float waterPercent = constrain((height - data.waterLevel) / height * 100.0, 0, 100);
    doc["waterPercent"] = waterPercent;

    doc["led"]  = digitalRead(2);
    doc["pump"] = digitalRead(17);
    doc["fan"]  = digitalRead(21);

    String payload;
    serializeJson(doc, payload);
    mqttPublish("smartfarm/data", payload);

    Serial.print("📤 MQTT -> ");
    Serial.println(payload);
  }

  delay(50);
}
