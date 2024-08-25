#include <EthernetENC.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
// #include "customLcd.h"
#include "globalPinDefinitions.h"
#include "globalVariable.h"
#include "customLcd.h"
#include "waterFlow.h"


void initiateEthernet();

void initiateMqtt(IPAddress mqttServer, char* sub_topic);
void callback(char* sub_topic, byte* payload, unsigned int length);
bool checkinternet(char* sub_topic);
void sendMsg(char* topic, char msg[]);
void mqloop();