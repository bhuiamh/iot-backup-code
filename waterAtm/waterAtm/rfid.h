
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>

#define byte uint8_t


//#define SS_PIN    20
//#define RST_PIN   21

void initiateRfid();
bool detectRfidReader();
bool cardDetected();
byte * getCardUid();
unsigned long int cardIDval();