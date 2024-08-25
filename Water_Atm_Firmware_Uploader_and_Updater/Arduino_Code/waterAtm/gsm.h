#include "globalPinDefinitions.h"
#include "timestamp32bits.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Arduino.h>

#define CONNACK "C"
#define SUBACK  "S"

void initiateGsm();
void enableDebugMode();
void disableDebugMode();
void resetGsm();
bool checkSIMPresence();
bool checkGsmNetwork();
char getNetworkStrength();
bool checkGprs();
bool setGprs();
bool shutTcp();
uint8_t syncTimeFromServer();
bool setModuleTime(char clockTime[]);
unsigned long generateTimestamp();
bool setBaudRate();
bool disableEcho();
void setBroker(String brokerURL);
void setTimeServer(String timeServer);
char publishRequestToBroker(unsigned long UId, unsigned long ts);
char publishBalanceToBroker(unsigned long UId, float waterDispensed, unsigned long ts);
void connectToBroker();
void subscribeToBroker(String topic);
void sendDataToBroker(String topic, String payload);
void sendTerminator();
bool gsmResponse(String expectedResponse, unsigned long int waitTime);
float getBalanceInfo();
float getCalibration();
void setDeviceID(String deviceID);
String getDeviceID();
String getBrokerResponse();
