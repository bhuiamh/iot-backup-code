#include "globalPinDefinitions.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

void initiateLcd();
void printCardIdOnLcd(byte * id);
void printCardRemovalMessage(float amount, float cost, float balance);
void printWelcomeMessage();
void printWaterStatus(float amount);
void printBalanceError();
void printAuthenticationError();
void printNetworkError();
void printWaitMessage();
void printConnectionError();
void printBootMessage();
void printGsmRebootMessage();
void printEthernetPresentMessage();
void printEthernetAbsentMessage();
void printRfidReaderError();

void printSimPresenceMessage();
void printSimAbsenceMessage();
void printNetworkOkMessage();
void printNetworkFailMessage();
void printNetworkStrengthNone();
void printNetworkStrengthLow();
void printNetworkStrengthMed();
void printNetworkStrengthHigh();
void printNetworkStrengthUnknown();
void printGprsSucessMessage();
void printGprsFailMessage();
void printClockSyncingMessage();
void printClockSucessMessage();
void printClockFailMessage();
void printClockFailMessageWithRetry();
void printGsmModuleError();
void printBrokerNotRespondingMessage();
void printGsmRebootedMessage();
void printGsmErrorZero();
void printGsmErrorOne();
void printGsmErrorTwo();
void printGsmErrorThree();
void printGsmErrorFour();
void printGsmErrorFive();
void printGsmErrorSix();
void printGsmErrorSeven();
void printGsmErrorEight();

void showErrorMessage(char msg[]);
