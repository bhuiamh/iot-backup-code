#include "globalPinDefinitions.h"
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>


#define SS_PIN    RFID_SS_PIN
#define RST_PIN   RFID_RESET_PIN

void initiateRfid();
bool detectRfidReader();
bool cardDetected();
byte * getCardUid();
