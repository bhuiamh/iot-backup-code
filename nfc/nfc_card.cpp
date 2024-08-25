#include "nfc_card.h"

String tagId="";
String tagId1="39 24 97 99";
String tagId2="C9 C9 71 24";
String tagId3="B7 80 6A 63";

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

int card_no=0;

void nfc_init(void)
{
  nfc.begin();
  Serial.println("nfc begin");
}

void nfc_read(void)
{
  if (nfc.tagPresent())
    {
        NfcTag tag = nfc.read();
        tag.print();
        tagId = tag.getUidString();

    }
    
    //delay();
}

int nfc_status_check(void)
{
  if (tagId == tagId1)
        {
          digitalWrite(13,HIGH);
          tagId="";
          card_no=1;

        }
  if (tagId == tagId2)
        {
          digitalWrite(13,LOW);
          tagId="";
          card_no=2;
        }
  return card_no;
}




