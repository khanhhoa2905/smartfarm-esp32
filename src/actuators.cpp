#include "actuators.h"
#include <ESP32Servo.h>

#define LED_PIN 2
#define PUMP_PIN 17
#define FAN_PIN 21
#define BUZZER_PIN 33   // còi logic ngược
#define SERVO_PIN 25



Servo myservo;
bool btnLightState = false, btnPumpState = false, btnFanState = false;

void initActuators() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);


  myservo.attach(SERVO_PIN);
  setBuzzer(false);
  setLED(false);
  setPump(false);
  setFan(false);
}

void setLED(bool s)   { digitalWrite(LED_PIN, s); }
void setPump(bool s)  { digitalWrite(PUMP_PIN, s); }
void setFan(bool s)   { digitalWrite(FAN_PIN, s); }
void setBuzzer(bool s){ digitalWrite(BUZZER_PIN, s ? LOW : HIGH); }  // LOW = bật còi
void setServoAngle(int angle) { myservo.write(angle); }


