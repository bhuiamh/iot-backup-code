#include "rfid.h"


bool success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
byte *cardID; // to store card UID
unsigned long int cardIDinDec = 0;

//Pin for SDl,SCL respectively in PN532

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

  return nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength,1000);
}

unsigned long int cardIDval()
{
    unsigned long cardIDPlaceHolder = 0;
    cardID = uid; // Fetching card ID

    cardIDPlaceHolder = *(cardID);
    cardIDinDec = cardIDPlaceHolder << 24;

    cardIDPlaceHolder = *(cardID + 1);
    cardIDinDec |= cardIDPlaceHolder << 16;

    cardIDPlaceHolder = *(cardID + 2);
    cardIDinDec |= cardIDPlaceHolder << 8;

    cardIDPlaceHolder = *(cardID + 3);
    cardIDinDec |= cardIDPlaceHolder;

    return cardIDinDec;
    
}

byte * getCardUid(){
  return uid;
}
