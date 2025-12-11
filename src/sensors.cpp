#include "sensors.h"
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_PIN 34
#define MQ135_PIN 35
#define FIRE_PIN 27
#define LDR_PIN 26
#define PIR_PIN 14
#define TRIG_PIN 12
#define ECHO_PIN 13

DHT dht(DHTPIN, DHTTYPE);

void initSensors() {
  dht.begin();
  pinMode(FIRE_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  return duration * 0.034 / 2;
}

SensorData readSensors() {
  SensorData d;
  d.temp = dht.readTemperature();
  d.hum = dht.readHumidity();
  
  d.soil = analogRead(SOIL_PIN);
  d.gas = analogRead(MQ135_PIN);
  d.soilPercent = constrain((1.0 - (float)data.soil / 4095.0) * 100.0, 0, 100);
data.gasPercent  = constrain(((float)data.gas / 4095.0) * 100.0, 0, 100);
  d.fire = digitalRead(FIRE_PIN);   // HIGH = cháy
  d.motion = digitalRead(PIR_PIN);
  d.light = digitalRead(LDR_PIN);
  d.waterLevel = readDistance();
  return d;
}
