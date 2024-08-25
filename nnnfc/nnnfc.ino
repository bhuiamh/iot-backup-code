#include <Wire.h>
#include <Adafruit_PN532.h>

// Create the PN532 instance using I2C
Adafruit_PN532 nfc(A5, A4);

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();
  Serial.println("nfc.begin() called");

  uint32_t versiondata = nfc.getFirmwareVersion();
  
  if (!versiondata) 
  {
    Serial.println("Didn't find PN53x board");
    while (1); // halt
  } 
  else 
  {
    Serial.print("Found PN53x board, firmware version: ");
    Serial.println(versiondata, HEX);
  }

  nfc.setPassiveActivationRetries(0xFF);
  Serial.println("Passive Activation Retries set");
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("SAMConfig set");
}

void loop(void) 
{
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) 
  {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");

    for (uint8_t i = 0; i < uidLength; i++) 
    {
      Serial.print(" 0x");
      Serial.print(uid[i], HEX);
    }
    Serial.println("");
    // Wait 1 second before continuing
    delay(1000);
  } 
  else 
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
  }
}
