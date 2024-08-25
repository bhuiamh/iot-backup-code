#include <Arduino.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <EthernetUdp.h>


#include "globalPinDefinitions.h"
#include "rfid.h"
#include "customLcd.h"
#include "waterFlow.h"
#include "solenoid.h"
#include "ethernet.h"
#include "globalVariable.h"

EthernetUDP Udp2;
NTPClient timeClient(Udp2);


void setup() {

  Serial.begin(9600);
  initiateLcd();
  initiateSolenoid();
  initiateEthernet();
  initiateRfid();
  timeClient.begin(); //for utc epoch timestamp

  initiateMqtt(mqttServer, subscription_topic);
}

void loop() {
  mqloop();
  timeClient.update();
 
  if (!cardDetected()) {

    if (cardPlaced) {

      cardPlaced = false;
      balanceError = false;

      if (!cardIsAuthorized) {
        sentCardUidToBroker = false;
        cardRead = false;
      }
      if (cardIsValid) {
        printCardRemovalMessage(amountOfWaterDispensed, costOfWater, cardBalance);
        sendcardbalance();
      }
    } else {
      printWelcomeMessage();
      delay(LCD_REFRESH_RATE);
    }

  }

  else {
   // checkinternet(subscription_topic);
    cardPlaced = true;  // Flag is made true for detecting the same card again while checking card presence
    if (!sentCardUidToBroker) {
      if (!cardRead) {

        Serial.print("Card ID: ");
        cardId = cardIDval();
        Serial.println(cardId);


        printClockSyncingMessage();
        timeStamp = timeClient.getEpochTime();
        Serial.println(timeStamp);
        printClockSucessMessage();

        //Serializing Json Document for Server
        config["UID"] = cardId;
        config["Time"] = timeStamp;
        config["MessageType"] = 1;
        config["Dispensed"] = 0;
        config["DeviceID"] = deviceId;

        serializeJson(config, msg_verify);
        Serial.println(msg_verify);
        printWaitMessage();

        //Sending Verification Request to Server
        if (checkinternet(subscription_topic)) {
          sendMsg(topic_verify, msg_verify);
          sentCardUidToBroker = true;
        } else {
          printConnectionError();
          delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
        }
        cardRead = true;
      }
    }

    // Works when callback is recieved
    if (cardIsValid) {

      cardIsAuthorized = true;
      unsigned long int cardDec = 0;
      unsigned long cardIDPlaceHolder = 0;


      cardDec = cardIDval();

      if (!flowSensorIsOn) {
        if (!balanceError) {
          openSolenoid();
          delay(250);
          initiateFlowSensor();
          flowSensorIsOn = true;
        }
      }

      //For detecting multiple cards
      if (cardId != cardDec) {
        sendcardbalance();
        printAuthenticationError();
        delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
        printWelcomeMessage();
      }


      //Calculation of the dispense water and show status in LCD
      if ((amountOfWaterDispensed < cardBalance) && cardIsAuthorized) {
        amountOfWaterDispensed = measureWaterDispensed();
        costOfWater = amountOfWaterDispensed * COST_PER_LITRE_OF_WATER;
        printWaterStatus(amountOfWaterDispensed, cardBalance);


      } else {
        closeSolenoid();
        //closeUv();
        stopFlowSensor();
        flowSensorIsOn = false;
        printBalanceError();
        balanceError = true;
      }
    } else {
      cardIsAuthorized = false;
    }
  }
}



void sendcardbalance() {

  closeSolenoid();
  stopFlowSensor();         // Stop water flow sensor calculation once the card is removed
  clearWaterFlowReading();  // Clearing the water flow sensor reading
  //cardBalance -= costOfWater;
  cardBalance -= amountOfWaterDispensed;
  if (cardBalance < 0) {
    cardBalance = 0.0;
  }
  // Displaying status on LCD
  timeStamp = timeClient.getEpochTime();
  config["Time"] = timeStamp;
  config["MessageType"] = 2;
  config["Dispensed"] = amountOfWaterDispensed;

  serializeJson(config, msg_dispense);
  Serial.println(msg_dispense);
 
  if (checkinternet(subscription_topic)) {
    sendMsg(topic_dispense, msg_dispense);

  } else {
    printNetError();
    delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
  }


  amountOfWaterDispensed = 0.0;
  costOfWater = 0.0;
  resetflags();
}


// if (interStatusReceived) {
//   interStatusReceived = false;
//   // if (mqclient.state() == 0) {
//   //   Serial.println("mq connected");
//   //   mqclient.subscribe(subscription_topic);
//     //mqclient.publish(topic_verify, msg_verify);
//   // }
//   sentCardUidToBroker = true;
// } else {
//   printConnectionError();
//   delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
// }
