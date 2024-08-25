#include "globalPinDefinitions.h"
#include <Arduino.h>

void sendUidToBroker(byte * id);
void doParse(char dataString[]);
void sendCardBalance(float balance);
void requestInternetStatus();
