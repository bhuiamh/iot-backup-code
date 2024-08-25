#include "nfc_card.h"
#include "global_variables.h"

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void nfc_init(void) {
    nfc.begin();
    Serial.println("NFC reader initialized");
}

void nfc_read(void) {
    if (nfc.tagPresent()) {
        NfcTag tag = nfc.read();
        tag.print();
        cardUID = tag.getUidString();  // Update global variable
        card_presented = true;
        Serial.print("presentend");
    } else {
        card_presented = false;
        cardUID = "";  // Clear the cardUID if no card is present
        Serial.print("not presentend");

    }
}
