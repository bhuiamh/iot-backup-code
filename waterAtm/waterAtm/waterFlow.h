#include <Arduino.h>
#include "globalPinDefinitions.h"

void calibrateFlowSensor(float calibratioFactor);
void initiateFlowSensor();
void pulseCounter();
void clearWaterFlowReading();
float measureWaterDispensed();
void stopFlowSensor();
