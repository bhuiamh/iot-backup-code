#include "globalPinDefinitions.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>



void initiateLcd();
//void printCardIdOnLcd(byte * id);
void printcard( unsigned long int cardIDinDec);
void printCardRemovalMessage(float amount, float cost, float balance);
void printWelcomeMessage();
void printWaterStatus(float amount, float cardBalance);
void printBalanceError();
void printAuthenticationError();
void printNetworkError();
void printWaitMessage();
void printConnectionError();
void printNetError();
void printRfidReaderError();



void printClockSyncingMessage();
void printClockSucessMessage();
void printClockFailMessage();
void printClockFailMessageWithRetry();

void printBrokerNotRespondingMessage();


void showErrorMessage(char msg[]);
void printToLCD(String message);
