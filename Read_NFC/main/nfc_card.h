#ifndef NFC_CARD_H
#define NFC_CARD_H

#include <Wire.h>
#include <PN532_I2C.h>
#include <NfcAdapter.h>

void nfc_init(void);
void nfc_read(void);

#endif
