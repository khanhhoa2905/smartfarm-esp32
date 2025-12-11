#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

struct SensorData {
    float temp;
    float hum;
    int soil;
    int gas;
    bool fire;
    bool motion;
    bool light;
    float waterLevel;
};

void initSensors();
SensorData readSensors();

#endif
