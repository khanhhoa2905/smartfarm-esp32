#include "automation.h"
#include "actuators.h"
#include "wifi_mqtt.h"

String allowedCards[] = {"D062C25F", "DC9749056"};
int servoAngle = 0;
bool servoDir = true;  // true = quay sang phải, false = quay sang trái
unsigned long lastServoMove = 0;

bool isAllowed(String id) {
  for (String s : allowedCards)
    if (id == s) return true;
  return false;
}

void runAutomation(SensorData d, String uid) {
  // ----- 1. FIRE DETECTION -----
  if (d.fire == LOW) {  // LOW = có lửa (Active Low)
    setBuzzer(true);
    setPump(true);
    servoDir = false;   // dừng servo
    mqttPublish("smartfarm/alert", "🔥 Fire detected");
  } else {
    setBuzzer(false);
    setPump(false);
  }

  // ----- 2. GAS & TEMPERATURE CONTROL -----
  bool fanOn = false;

  if (d.gas > 3000) {
    fanOn = true;
    mqttPublish("smartfarm/alert", "💨 High gas detected");
  }

  if (d.temp > 35 && d.motion) {
    fanOn = true;
    mqttPublish("smartfarm/alert", "🔥 High temperature detected");
  }

  setFan(fanOn);

  // ----- 3. LIGHT & MOTION CONTROL -----
  if (d.motion && d.light == LOW)
    setLED(true);
  else
    setLED(false);

  // ----- 4. WATER LEVEL & SOIL MOISTURE -----
  float height = 10.0;  // cm
  float waterPercent = constrain((height - d.waterLevel) / height * 100.0, 0, 100);

  if (d.soil >= 2000 && waterPercent > 20 && waterPercent < 95) {
    setPump(true);
    mqttPublish("smartfarm/alert", "💧 Pump ON: soil dry, water " + String(waterPercent, 1) + "%");
  } else if (waterPercent <= 20) {
    setPump(false);
    mqttPublish("smartfarm/alert", "🚱 Low water level (" + String(waterPercent, 1) + "%)");
  } else if (waterPercent >= 95 || d.soil < 2000) {
    setPump(false);
  }

  // ----- 5. SERVO SG90: quay qua lại 0–180° -----
  // SG90 chỉ quay 180°, nên ta cho nó lắc qua lắc lại.
  if (d.fire != LOW) {  // chỉ quay khi không cháy
    if (millis() - lastServoMove > 20) {
      lastServoMove = millis();

      if (servoDir) servoAngle += 2;
      else servoAngle -= 2;

      // Khi tới giới hạn thì đảo chiều
      if (servoAngle >= 180) servoDir = false;
      if (servoAngle <= 0)   servoDir = true;

      setServoAngle(servoAngle);
    }
  }

  // ----- 6. RFID ACCESS -----
  if (uid != "") {
    if (isAllowed(uid)) {
      mqttPublish("smartfarm/rfid", "✅ Access OK");
      mqttPublish("smartfarm/alert", "Authorized card detected");
    } else {
      mqttPublish("smartfarm/rfid", "❌ Access Denied");
      mqttPublish("smartfarm/alert", "Unauthorized card detected");
    }
  }
}
