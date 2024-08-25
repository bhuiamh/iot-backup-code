#include "globalVariable.h"



bool cardPlaced = false; // to avoid same card presence checking error
bool sentCardUidToBroker = false;

float amountOfWaterDispensed = 0.0; // variable for storing amount of water dispensed
float costOfWater = 0.0;            // variable for calculating water cost
double cardBalance = 0.00;   // variable to store the remaining balance on card


bool rfidReaderOk = false;
bool cardRead = false;
bool interStatusReceived = false;
bool cardIsValid = false; // test variable to generate acknowledgement condition from broker
bool cardIsAuthorized = false;
bool balanceError = false;
bool flowSensorIsOn = false; // flag to check if the flow sensor has been turned on
bool callbackReceived =false;

JsonDocument reply;
unsigned long timeStamp=0;



char* topic_verify="request/verify";    //server topic for verification
char* topic_dispense="request/dispense"; //server topic for dispensed water status



//char* sub_topic = "response/";
//char* msg1="{\"StatusCode\": 0, \"Water_Balance\": 0, \"UID\": 3554321946L, \"Time\": 1715567087, \"MessageType\": 1, \"Message\": 0, \"Balance\": 0, \"Dispensed\": 0, \"DeviceID\": \"DV124\"}";
// char* msg2="{\"StatusCode\": 0, \"Water_Balance\": 0, \"UID\": 3554321946L, \"Time\": 1715658898, \"MessageType\": 2, \"Message\": 0, \"Balance\": 0, \"Dispensed\": 0.54, \"DeviceID\": \"DV124\"}";


unsigned long int cardId=0;


String brokerMsg; 
JsonDocument config;
JsonDocument docs;

char msg_verify[256]; 
char msg_dispense[256];


//IPAddress mqttServer(192, 168, 0, 119);

void resetflags() {
  sentCardUidToBroker = false;
  cardRead = false;
  cardIsValid = false;
  cardIsAuthorized = false;
  flowSensorIsOn = false;
}

IPAddress mqttServer(13, 214, 1, 75);
String deviceId= "DV124";

String topic_sub = "response/" + deviceId;
const char* subscription_topic = topic_sub.c_str();

