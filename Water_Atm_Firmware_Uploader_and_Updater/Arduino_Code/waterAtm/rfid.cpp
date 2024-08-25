#include "rfid.h"

bool success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void initiateRfid(){
  nfc.begin();
  nfc.SAMConfig();
}

bool detectRfidReader(){
  return nfc.getFirmwareVersion();
}

bool cardDetected(){
  return nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
}

byte * getCardUid(){
  return uid;
}
