#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H
#include <Arduino.h>

void initWifiMQTT();
void mqttLoop();
void mqttPublish(const char* topic, String payload);

#endif
