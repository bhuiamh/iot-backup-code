
#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <EthernetENC.h>

extern  bool cardPlaced ; // to avoid same card presence checking error
extern  bool sentCardUidToBroker; //to check if verification msg is sent

extern  float amountOfWaterDispensed; // variable for storing amount of water dispensed
extern  float costOfWater ;           // variable for calculating water cost
extern double cardBalance ;   // variable to store the remaining balance on card


extern bool rfidReaderOk ;
extern bool cardRead ;
extern bool interStatusReceived ; //internet status
extern bool cardIsValid; // test variable to generate acknowledgement condition from broker
extern bool cardIsAuthorized ;
extern bool balanceError ;
extern bool flowSensorIsOn;  // flag to check if the flow sensor has been turned on
extern bool callbackReceived ;
extern JsonDocument reply;
extern  unsigned long timeStamp;



extern  char* topic_verify;
extern  char* topic_dispense;



//char* sub_topic = "response/";
//char* msg1="{\"StatusCode\": 0, \"Water_Balance\": 0, \"UID\": 3554321946L, \"Time\": 1715567087, \"MessageType\": 1, \"Message\": 0, \"Balance\": 0, \"Dispensed\": 0, \"DeviceID\": \"DV124\"}";
// char* msg2="{\"StatusCode\": 0, \"Water_Balance\": 0, \"UID\": 3554321946L, \"Time\": 1715658898, \"MessageType\": 2, \"Message\": 0, \"Balance\": 0, \"Dispensed\": 0.54, \"DeviceID\": \"DV124\"}";


extern unsigned long int cardId;


extern String brokerMsg; 
extern JsonDocument config;
extern JsonDocument docs;

extern char msg_verify[256]; 
extern char msg_dispense[256];



extern IPAddress mqttServer;
extern String deviceId;

extern String topic_sub ;
extern const char* subscription_topic;


void resetflags();

#endif

