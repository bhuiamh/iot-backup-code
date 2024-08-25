#include "globalPinDefinitions.h"
#include "rfid.h"
#include "customLcd.h"
#include "waterFlow.h"
#include "solenoid.h"
#include "uv.h"
#include "parser.h"
#include "gsm.h"
#include "disableCan.h"

bool cardPlaced = false;  // to avoid same card presence checking error
bool sentCardUidToBroker = false;
byte * cardID; // to store card UID
unsigned long int cardIDinDec = 0;
unsigned long unixTimestamp = 0;

float amountOfWaterDispensed = 0.0; // variable for storing amount of water dispensed
float costOfWater = 0.0;  // variable for calculating water cost

bool simPresence = false;
bool timeSynced = false;
bool ethernetPresent = true;
bool systemBoot = false;
bool bootMessageShown = false;
bool welcomeMessageShown = false;
bool rfidReaderOk = false;
bool cardRead = false;
bool interStatusReceived = false;
bool cardIsValid = false; // test variable to generate acknowledgement condition from broker
bool cardIsAuthorized = false;
bool balanceError = false;
bool flowSensorIsOn = false;  // flag to check if the flow sensor has been turned on
bool gsmNotInit = true; // flag to check if gsm module has been initialized after microprocessor restarts
double cardBalance = 0.00;  // variable to store the remaining balance on card
char cardBalanceInfo[MAX_BALANCE_LENGTH];
char configInfo[MAX_BALANCE_LENGTH];
char brokerMessage[MAX_MESSAGE_LENGTH];
char brokerURL[MAX_BROKER_LENGTH];
char deviceID[MAX_DEVICE_ID_LENGTH];  // To get the device ID from microprocessor
int devIdIndex = 0;
int gsmState = 0;
uint8_t brokerStringCounter = 0;

char receivedString[MAX_UART_DATA_LENGTH]; // For storing the received data from uProcessor
char receivedChar = '\0'; // For storing one character from uProcessor
uint8_t uartDataCounter = 0;  // For counting the indexes of character array

void setup() {
  disableCANBus();
  Serial.begin(115200);
  initiateLcd();
  initiateSolenoid();
  initiateUv();  
}

void loop() {

  if(Serial.available() > 0){
    receivedChar = Serial.read();
    
    if(receivedChar == '\n'){
      receivedString[uartDataCounter] = '\n';
      receivedChar = 0x00;
      uartDataCounter = 0;

    switch(receivedString[0]){
      case 'D':
        switch(receivedString[1]){
          case 'E':
            enableDebugMode();
          break;

          case 'D':
            disableDebugMode();
          break;
        }
      break;
      
      case 'T':
      break;

      case 'O':
        systemBoot = true;      
      break;

      case 'E':
        switch(receivedString[1]){
          case 'Y':
            ethernetPresent = true;
            printEthernetPresentMessage();
            delay(LCD_REFRESH_RATE);
          break;

          case 'N':
            ethernetPresent = false;
            printEthernetAbsentMessage();
            //Serial.println("A");  // Only for GSM debugging
          break;
        }
      interStatusReceived = true;
      break;

      case 'C':
        switch(receivedString[1]){
          case 'R':
            switch(receivedString[2]){
              case 'Y':
                cardIsValid = true;
                cardIsAuthorized = true;
              break;

              case 'N':
                cardIsValid = false;
                cardIsAuthorized = false;
                //printAuthenticationError();
                showErrorMessage(brokerMessage);
                delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
              break;

              case 'T':
                cardIsValid = false;
                cardIsAuthorized = false;
                printConnectionError();
                delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
              break;
            }
          break;

          case 'I':
            switch(receivedString[2]){
              case 'B':
                for(uint8_t i = 0; i < MAX_BALANCE_LENGTH; i++){
                  cardBalanceInfo[i] = receivedString[3+i];
                  if(cardBalanceInfo[i] == '\n'){
                    cardBalance = atof(cardBalanceInfo);
                    break;
                  }
                }
              break;

              case 'C':
                for(uint8_t i = 0; i < MAX_BALANCE_LENGTH; i++){
                  configInfo[i] = receivedString[3+i];
                  if(configInfo[i] == '\n'){
                    calibrateFlowSensor(atof(configInfo));
                    break;
                  }
                }
              break;

              case 'N':
                for(uint8_t i = 0; i < MAX_DEVICE_ID_LENGTH; i++){
                  deviceID[i] = receivedString[3+i];
                }
                setDeviceID(deviceID);
              break;

              case 'A':
                for(uint8_t i = 0; i < MAX_BROKER_LENGTH; i++){
                  brokerURL[i] = receivedString[3+i];
                  if(brokerURL[i] == '\"'){
                    brokerStringCounter += 1;
                    if(brokerStringCounter == 2){
                      //brokerURL[i] == NULL;
                      break;
                    }
                  }
                }
                brokerStringCounter = 0;
                setBroker(brokerURL);
              break;

              case 'M':
                for(uint8_t i = 0; i < MAX_MESSAGE_LENGTH; i++){
                  brokerMessage[i] = receivedString[3+i];
                  if(brokerMessage[i] == '\n'){
                    brokerMessage[i] = NULL;
                    break;
                  }
                }
                if(!systemBoot){
                  showErrorMessage(brokerMessage);
                  delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
                }
              break;
            }
          break;
        }
      break;
    }
      
    }
    else{
      receivedString[uartDataCounter] = receivedChar;
      uartDataCounter += 1;
    }
  }
    
  if(!systemBoot){
    if(!bootMessageShown){
      printBootMessage();
      bootMessageShown = true;
    }
  }
  else{
    if(gsmNotInit){
      initiateGsm();
      setBaudRate();
      disableEcho();

      if(checkSIMPresence()){
        simPresence = true;
        printSimPresenceMessage();
        delay(TIME_TO_SHOW_STATE_UPDATES);

        if(checkGsmNetwork()){
          printNetworkOkMessage();
        }
        //else printNetworkFailMessage();
        delay(TIME_TO_SHOW_STATE_UPDATES);

        switch(getNetworkStrength()){
          case 'N':
            printNetworkStrengthNone();
          break;

          case 'L':
            printNetworkStrengthLow();
          break;

          case 'M':
            printNetworkStrengthMed();
          break;

          case 'H':
            printNetworkStrengthHigh();
          break;

          case 'U':
            printNetworkStrengthUnknown();
          break;
        }
        delay(TIME_TO_SHOW_STATE_UPDATES);        

        if(checkGprs()){
          printGprsSucessMessage();
        }
        else{
          if(setGprs()){
            if(checkGprs()){
              printGprsSucessMessage();
            }
            else printGprsFailMessage();
          }
          else printGprsFailMessage();
        }
        delay(TIME_TO_SHOW_STATE_UPDATES);

        printClockSyncingMessage();
        delay(TIME_TO_SHOW_STATE_UPDATES);
        
        for(int retryAttempts = 0; retryAttempts < 3; retryAttempts++){
          gsmState = syncTimeFromServer();
          if(gsmState == 5){
            timeSynced = true;
            break;
          }
          else{
            timeSynced = false;
            printClockFailMessageWithRetry();
            delay(TIME_TO_SHOW_STATE_UPDATES);
          }
//          switch(syncTimeFromServer()){
//            case 0:
//            printGsmErrorZero();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 1:
//            printGsmErrorOne();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 2:
//            printGsmErrorTwo();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 3:
//            printGsmErrorThree();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 4:
//            printGsmErrorFour();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 5:
//            //printGsmErrorFive();
//            timeSynced = true;
//            //delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 6:
//            printGsmErrorSix();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 7:
//            printGsmErrorSeven();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//
//            case 8:
//            printGsmErrorEight();
//            timeSynced = false;
//            delay(TIME_TO_SHOW_STATE_UPDATES);
//            break;
//        }        
        }
        
        if(!timeSynced){
          printClockFailMessage();
        }
        else printClockSucessMessage();
        delay(TIME_TO_SHOW_STATE_UPDATES);
      }
      else{
        simPresence = false;
        printSimAbsenceMessage();
      }
      delay(TIME_TO_SHOW_STATE_UPDATES);

      gsmNotInit = false;
      }

    if(!detectRfidReader()){
      rfidReaderOk = false;
      printRfidReaderError();
      delay(LCD_REFRESH_RATE);
    }
    else{
      if(!rfidReaderOk){
        initiateRfid();
        rfidReaderOk = true;
      }
      if(!cardDetected()){
        if(cardPlaced){
          welcomeMessageShown = false;
          cardPlaced = false;
          balanceError = false;
      
          if(!cardIsAuthorized){
            sentCardUidToBroker = false;
            cardRead = false;
          }
          if(cardIsValid){
            sentCardUidToBroker = false;
            cardRead = false;
            cardIsValid = false;
            cardIsAuthorized = false;
            closeSolenoid();
            //closeUv();
            stopFlowSensor(); // Stop water flow sensor calculation once the card is removed
            flowSensorIsOn = false;
            clearWaterFlowReading();  // Clearing the water flow sensor reading
            //cardBalance -= costOfWater;
            cardBalance -= amountOfWaterDispensed;
            if(cardBalance < 0){
              cardBalance = 0.0;
            }
            printCardRemovalMessage(amountOfWaterDispensed, costOfWater, cardBalance); // Displaying status on LCD
        
        
            if(!ethernetPresent){
              if(simPresence){
                if(timeSynced){
                  unixTimestamp = generateTimestamp();
                  delay(250);
                  switch(publishBalanceToBroker(cardIDinDec, amountOfWaterDispensed, unixTimestamp)){
                    case 0:
                    printGsmModuleError();
                  break;

                  case 1:
                    //Serial.println("Communication successful!");
                  break;

                  case 2:
                    //Serial.println("Invalid!");
                  break;

                  case 3:
                    printBrokerNotRespondingMessage();
                  break;

                  case 4:
                    printGsmRebootedMessage();
                  break;
                  }
                }
                else{
                  printClockFailMessage();
                  delay(TIME_TO_SHOW_STATE_UPDATES);
                }
              }
              else{
                printSimAbsenceMessage();
                delay(TIME_TO_SHOW_STATE_UPDATES);
              }
            }
            else{
              sendCardBalance(amountOfWaterDispensed);
              delay(TIME_TO_SHOW_BALANCE);
            }

            amountOfWaterDispensed = 0.0;
            costOfWater = 0.0;
          } 
        }
        else{
          printWelcomeMessage();
          delay(LCD_REFRESH_RATE);
        }
      }
      else{
        cardPlaced = true;  // Flag is made true for detecting the same card again while checking card presence
        if(!sentCardUidToBroker){
          if(!cardRead){
          unsigned long cardIDPlaceHolder = 0;
          cardID = getCardUid();  // Fetching card ID
      
          cardIDPlaceHolder = *(cardID);
          cardIDinDec = cardIDPlaceHolder << 24;
    
          cardIDPlaceHolder = *(cardID+1);
          cardIDinDec |= cardIDPlaceHolder << 16;
    
          cardIDPlaceHolder = *(cardID+2);
          cardIDinDec |= cardIDPlaceHolder << 8;
    
          cardIDPlaceHolder = *(cardID+3);
          cardIDinDec |= cardIDPlaceHolder;
    
          printWaitMessage();
          requestInternetStatus();
          cardRead = true;
        }

        if(interStatusReceived){
          interStatusReceived = false;
          if(!ethernetPresent){
            if(simPresence){
              if(timeSynced){
                unixTimestamp = generateTimestamp();
                delay(250);
                switch(publishRequestToBroker(cardIDinDec, unixTimestamp)){
                case 0:
                  cardIsValid = false;
                  cardIsAuthorized = false;
                  printGsmModuleError();
                  delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
                  welcomeMessageShown = false;
                  sentCardUidToBroker = true;
                break;
                    
                case 1:
                  cardIsValid = true;
                  cardIsAuthorized = true;
                  cardBalance = getBalanceInfo();
                  calibrateFlowSensor(getCalibration());
                  welcomeMessageShown = false;
                  sentCardUidToBroker = true;
                break;
      
                case 2:
                  cardIsValid = false;
                  cardIsAuthorized = false;
                  getBrokerResponse().toCharArray(brokerMessage, MAX_BROKER_LENGTH);
                  showErrorMessage(brokerMessage);
                  delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
                  welcomeMessageShown = false;
                  sentCardUidToBroker = true;
                break;
      
                case 3:
                  printBrokerNotRespondingMessage();
                  delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
                  welcomeMessageShown = false;
                  sentCardUidToBroker = true;
                break;

                case 4:
                  printGsmRebootedMessage();
                  delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
                  welcomeMessageShown = false;
                  sentCardUidToBroker = true;
                break;
                }
              }
              else{
                printClockFailMessage();
                delay(TIME_TO_SHOW_STATE_UPDATES);
              }
            }
            else{
              printSimAbsenceMessage();
              delay(TIME_TO_SHOW_STATE_UPDATES);
            }
          }
          else{
            sendUidToBroker(cardID); // Sending Card UID to broker
            sentCardUidToBroker = true;
          }
        }
        }
  
        if(cardIsValid){
          welcomeMessageShown = false;
          cardIsAuthorized = true;
    
        if(!flowSensorIsOn){
          if(!balanceError){
            openSolenoid();
            //openUv();
            delay(250);
            initiateFlowSensor();
            flowSensorIsOn = true;
          } 
        }
    
        if((amountOfWaterDispensed < cardBalance)){
          amountOfWaterDispensed = measureWaterDispensed();
          costOfWater = amountOfWaterDispensed * COST_PER_LITRE_OF_WATER;
          printWaterStatus(amountOfWaterDispensed);
        }
        else{
          closeSolenoid();
          //closeUv();
          stopFlowSensor();
          flowSensorIsOn = false;
          printBalanceError();
          balanceError = true;
        }
      }
      else{
        cardIsAuthorized = false;
        welcomeMessageShown = false;
      }
      }
    }
  }
}
