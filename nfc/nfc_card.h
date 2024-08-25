#ifndef NFC_CARD_H
#define NFC_CARD_H

#include <Arduino.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>


void nfc_init(void);
void nfc_read(void);
int nfc_status_check(void);



#endif