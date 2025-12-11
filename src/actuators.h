#ifndef ACTUATORS_H
#define ACTUATORS_H
#include <Arduino.h>

void initActuators();
void setLED(bool s);
void setPump(bool s);
void setFan(bool s);
void setBuzzer(bool s);
void setServoAngle(int angle);
void readButtons();
bool getButtonLight();
bool getButtonPump();
bool getButtonFan();

#endif
