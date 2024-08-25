#include "waterFlow.h"

long numberOfPulses = 0;
float waterDispensed = 0.0;
float multiplicationFactor = FLOW_SENSOR_MULTIPLICAION_FACTOR;

void initiateFlowSensor(){
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
}

void stopFlowSensor(){
 detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN)); 
}

void pulseCounter(){
  numberOfPulses += 1;
}

void calibrateFlowSensor(float calibratioFactor){
  multiplicationFactor = calibratioFactor * FLOW_SENSOR_MULTIPLICAION_FACTOR;
}

float measureWaterDispensed(){
  waterDispensed = numberOfPulses * multiplicationFactor;
  return waterDispensed;
}

void clearWaterFlowReading(){
  numberOfPulses = 0;
}

