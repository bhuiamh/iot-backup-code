#include "gsm.h"

StaticJsonBuffer<512> jsonBuffer;
timestamp32bits stamp = timestamp32bits();
SoftwareSerial mySerial (GSM_RX, GSM_TX);

// DEBUG MODE
bool debugMode = false;

// GSM COMMANDS

double balanceInfo = 0.0;
float calibrationInfo = 1.0;
String brokerResponseMessage = "\0";

String mqttBroker = "\"broker.drinkwellatm.com\"";
//String mqttBroker = "\"3.0.109.66\"";
String timeServerUrl = "\"3.129.251.210\"";
String gsmTcpTimeServer = "AT+CIPSTART=\"TCP\"," + timeServerUrl + ",\"80\"\r\n";
String timeServerApi = "GET /webapi/get-current-time\r\n";
String gsmTcp = "AT+CIPSTART=\"TCP\"," + mqttBroker + ",\"1883\"\r\n";
char gsmCmdCounter = 0;
char newLineCounter = 0;
bool responseFromBroker = false;
bool networkStrenghtRead = false;
bool storeStrengthValue = false;
bool strengthReadComplete = false;
String strengthString = "\0";
int strengthValue = 0;
int ackCounter = 0;

String gsmCmd[] = {
  "ATE0\r\n",
  "AT+CSTT=\"internet\"\r\n",
  "AT+CIICR\r\n",
  gsmTcp,
  "AT+CIPSEND\r\n",
  "AT+CIPCLOSE\r\n",
  "AT+CREG?\r\n",
  "AT+CSQ\r\n",
  "AT+CGATT?\r\n",
  "AT+CGATT=1\r\n",
  "AT+CIPSHUT\r\n",
  "AT+IPR=4800\r\n",
  gsmTcpTimeServer, // 12
  timeServerApi, // 13
  "AT+CPIN?\r\n" // 14
  };

// GSM COMMANDS

String requestString = "{\"UID\":22843950,\"DeviceID\":\"DM001\",\"Time\":1583312245,\"MessageType\":1}";
String updateString = "{\"UID\":22843950,\"DeviceID\":\"test\",\"Time\":1583312245,\"MessageType\":2,\"Dispensed\":0}";
String requestTopic = "request/verify";
String dispenseTopic = "request/dispense";
String devID = "DM001";
String subscribeTopic = "response/" + devID;
char receivedJson[300]; // To store the response data received from broker
long int receivedJsonIndex = 0;
char identifierCounter = 0; // To count the number of '{' and '}' received
String jsonToPrint;

char receivedByte = '\0';

String myString;

JsonObject& root = jsonBuffer.parseObject(requestString);
JsonObject& rootUpdate = jsonBuffer.parseObject(updateString);

void enableDebugMode(){
  debugMode = true;
}

void disableDebugMode(){
  debugMode = false;
}

void setBroker(String brokerURL){
  mqttBroker = brokerURL;
  gsmTcp = "AT+CIPSTART=\"TCP\"," + mqttBroker + ",\"1883\"\r\n";
  gsmCmd[3] = gsmTcp;
}

void setTimeServer(String timeServer){
  timeServerUrl = timeServer;
  gsmTcpTimeServer = "AT+CIPSTART=\"TCP\"," + timeServerUrl + ",\"80\"\r\n";
  gsmCmd[12] = gsmTcpTimeServer;
}

void setDeviceID(String deviceID){
  devID = deviceID;
  subscribeTopic = "response/" + devID;
}

String getDeviceID(){
  return devID;
}

void initiateGsm(){
  mySerial.begin(9600);
  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);
}

void resetGsm(){
  digitalWrite(GSM_RESET, LOW);
  delay(500);
  digitalWrite(GSM_RESET, HIGH);
  delay(GSM_WAKEUP_TIME);
}

float getBalanceInfo(){
  return balanceInfo;
}

float getCalibration(){
  return calibrationInfo;
}

String getBrokerResponse(){
  return brokerResponseMessage;
}

bool checkSIMPresence(){
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;

  while(!gsmCmdResponse && ((millis() - previousTime) < GSM_WAKEUP_TIME)){
    delay(1000);
    if(debugMode){
      Serial.print(gsmCmd[14]);
    }
    mySerial.print(gsmCmd[14]);

    if(gsmResponse("+CPIN: READY", 5)){
      gsmCmdResponse = true;
      while(!gsmResponse("OK", 5));
    }
    else gsmCmdResponse = false;
  }

  if(gsmCmdResponse){
    return true;
  }
  else return false;
}

bool checkGsmNetwork(){
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;

  while(!gsmCmdResponse && ((millis() - previousTime) < GSM_WAKEUP_TIME)){
    delay(1000);
    if(debugMode){
      Serial.print(gsmCmd[6]);
    }
    mySerial.print(gsmCmd[6]);

    if(gsmResponse("+CREG: 0,1", 5)){
      gsmCmdResponse = true;
      while(!gsmResponse("OK", 5));
    }
    else gsmCmdResponse = false;
  }

  if(gsmCmdResponse){
    return true;
  }
  else return false;
}

char getNetworkStrength(){
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;
  networkStrenghtRead = true;

  while(!gsmCmdResponse && ((millis() - previousTime) < GSM_WAKEUP_TIME)){
    delay(1000);
    if(debugMode){
      Serial.print(gsmCmd[7]);
    }
    mySerial.print(gsmCmd[7]);
    if(gsmResponse(",", 5)){
      gsmCmdResponse = true;
      //Serial.println(strengthValue);
      if(strengthValue <= 2){
        return 'N';
      }
      else if(strengthValue > 2 && strengthValue <= 13){
        return 'L';
      }
      else if(strengthValue > 13 && strengthValue <= 25){
        return 'M';
      }
      else if(strengthValue > 25 && strengthValue < 32){
        return 'H';
      }
      else if(strengthValue > 32){
        return 'U';
      }
    }
  }
}

bool checkGprs(){
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;

  while(!gsmCmdResponse && ((millis() - previousTime) < GSM_WAKEUP_TIME)){
    delay(1000);
    if(debugMode){
      Serial.print(gsmCmd[8]);
    }
    mySerial.print(gsmCmd[8]);

    if(gsmResponse("+CGATT: 1", 5)){
      gsmCmdResponse = true;
      while(!gsmResponse("OK", 5));
    }
    else gsmCmdResponse = false;
  }

  if(gsmCmdResponse){
    return true;
  }
  else return false;
}

bool setGprs(){
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;

  while(!gsmCmdResponse && ((millis() - previousTime) < GSM_WAKEUP_TIME)){
    delay(1000);
    if(debugMode){
      Serial.print(gsmCmd[9]);
    }
    mySerial.print(gsmCmd[9]);

    if(gsmResponse("OK", 5)){
      gsmCmdResponse = true;
    }
    else gsmCmdResponse = false;
  }

  if(gsmCmdResponse){
    return true;
  }
  else return false;
}

bool shutTcp(){
  if(debugMode) Serial.print(gsmCmd[10]);
  mySerial.print(gsmCmd[10]);
  if(gsmResponse("SHUT OK", 5)){
    delay(50);
    if(checkGprs()){
      return false;
    }
    else{
      if(setGprs()){
        delay(50);
        if(checkGprs()){
          delay(50);
          return false;
        }
        else{
          return false;
        }
      }
      else{
        return false;
      }
    }
  }
  else{
    return false;
  } 
}

uint8_t syncTimeFromServer(){
  receivedJsonIndex = 0;
  identifierCounter = 0;
  receivedByte = '\0';
  
  if(debugMode) Serial.print(gsmCmd[0]);
  mySerial.print(gsmCmd[0]);
  if(gsmResponse("OK", 5)){
    if(debugMode) Serial.print(gsmCmd[12]);
    mySerial.print(gsmCmd[12]);
    if(gsmResponse("OK", 5)){
      if(gsmResponse("CONNECT OK", 10)){
        if(debugMode) Serial.print(gsmCmd[4]);
        mySerial.print(gsmCmd[4]);
        if(gsmResponse(">", 10)){
          if(debugMode) Serial.print(gsmCmd[13]);
          mySerial.print(gsmCmd[13]);
          sendTerminator();
        }
        else{
          if(debugMode) Serial.println("GSM Error: 8");
          return 8;
        }
        if(!(gsmResponse("SEND OK", 10))){
          if(debugMode) Serial.println("GSM Error: 3");
          return 3;
        }
      }
      else{
        shutTcp();
        if(debugMode) Serial.println("GSM Error: 2");
        return 2;
      }
    }
    else{
      shutTcp();
      if(debugMode) Serial.println("GSM Error: 1");
      return 1;
    }
  }
  else{
    shutTcp();
    if(debugMode) Serial.println("GSM Error: 0");
    return 0;
  }

  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;
    
  while((!gsmCmdResponse) && ((millis() - previousTime) < BROKER_RESPONSE_TIME)){
      
  if(mySerial.available() > 0){
    receivedByte = mySerial.read();
    //if(debugMode) Serial.print(receivedByte);

    if(receivedByte > 31 && receivedByte < 127){
      if(receivedByte == '{'){
        identifierCounter = 1;
      }
      else if(receivedByte == '}'){
        identifierCounter = 2;
      }
      if(identifierCounter == 1){
        receivedJson[receivedJsonIndex] = receivedByte;
        receivedJsonIndex += 1;
      }
      else if(identifierCounter == 2){
        receivedJson[receivedJsonIndex] = receivedByte;
        receivedJson[receivedJsonIndex + 1] = '\0';
        receivedJsonIndex = 0;
        identifierCounter = 0;
        if(debugMode){
          Serial.println();
          Serial.print("Received from broker: ");
          Serial.println(receivedJson);
        }

        mySerial.print(gsmCmd[5]);
        if(!(gsmResponse("CLOSE OK", 10))){
          if(debugMode) Serial.println("GSM Error: 4");
          shutTcp();
          return 4;
        }

        StaticJsonBuffer<512> jsonBuffer;
        JsonObject& jsonResponse = jsonBuffer.parseObject(receivedJson);

        jsonBuffer.clear();
        gsmCmdResponse = true;

        for(int retryAttempt = 0; retryAttempt < 3; retryAttempt++){
          if(setModuleTime(jsonResponse["UtcDateTime"])){
            if(debugMode) Serial.println("Time sync Success");
            return 5;
          }
          else{
            if(debugMode) Serial.println("GSM Error: 6");
            return 6;
          }
        }
      }      
    }
  }
 }

 if(!gsmCmdResponse){
  if(debugMode) Serial.println("GSM Error: 7");
  return 7;
    myString = "\0";
    if(debugMode){
      Serial.println("No response from broker!");
      Serial.print(gsmCmd[5]);
    }
    mySerial.print(gsmCmd[5]);
    gsmResponse("CLOSE OK", 10);
  }
}

bool setModuleTime(char clockTime[]){
  if(debugMode){
    Serial.print("AT+CCLK=");
    Serial.print("\"");
    Serial.print(clockTime);
    Serial.println("\"");
  }
  mySerial.print("AT+CCLK=");
  mySerial.print("\"");
  mySerial.print(clockTime);
  mySerial.print("\"");
  mySerial.print("\r\n");

  if(gsmResponse("OK", 5)){
    return true;  
  }
  else{
    return false;
  }
}

unsigned long generateTimestamp(){
  unsigned long timeStampValue = 0;
  String placeholder;
  unsigned char year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;

  if(debugMode) Serial.print("AT+CCLK?\r\n");
  mySerial.print("AT+CCLK?\r\n");
    
  while((!gsmCmdResponse) && ((millis() - previousTime) < GSM_WAKEUP_TIME)){    
    if(mySerial.available() > 0){
      receivedByte = mySerial.read();
      if(debugMode) Serial.print(receivedByte);

      if(receivedByte == '"'){
        identifierCounter += 1;
      }
      if(identifierCounter == 1){
        if(receivedByte != '"'){
          receivedJson[receivedJsonIndex] = receivedByte;
          receivedJsonIndex += 1;
        }
      }
      else if(identifierCounter == 2){
        receivedJson[receivedJsonIndex] = '\0';
        receivedJsonIndex = 0;
        identifierCounter = 0;
        if(debugMode){
          Serial.println();
          Serial.print("RTC Time: ");
          Serial.println(receivedJson);
        }
        gsmCmdResponse = true;
      }
    }
  }

  if(gsmResponse("OK", 5)){
    gsmCmdResponse = true;
  }
  else gsmCmdResponse = false;

  placeholder = receivedJson[0];
  placeholder += receivedJson[1];
  year = placeholder.toInt();
  
  placeholder = receivedJson[3];
  placeholder += receivedJson[4];
  month = placeholder.toInt();

  placeholder = receivedJson[6];
  placeholder += receivedJson[7];
  day = placeholder.toInt();

  placeholder = receivedJson[9];
  placeholder += receivedJson[10];
  hour = placeholder.toInt();

  placeholder = receivedJson[12];
  placeholder += receivedJson[13];
  minute = placeholder.toInt();

  placeholder = receivedJson[15];
  placeholder += receivedJson[16];
  second = placeholder.toInt();

  timeStampValue = stamp.timestamp(year,month,day,hour,minute,second);

  return timeStampValue;
}

bool disableEcho(){
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;

  while(!gsmCmdResponse && ((millis() - previousTime) < GSM_WAKEUP_TIME)){
    delay(1000);
    if(debugMode) Serial.print(gsmCmd[0]);
    mySerial.print(gsmCmd[0]);

    if(gsmResponse("OK", 2)){
      gsmCmdResponse = true;
    }
    else gsmCmdResponse = false;
  }

  if(gsmCmdResponse){
    return true;
  }
  else return false;
}

bool setBaudRate(){
  unsigned long previousTime = millis();
  bool gsmCmdResponse = false;

  while(!gsmCmdResponse && ((millis() - previousTime) < GSM_WAKEUP_TIME)){
    delay(1000);
    if(debugMode) Serial.println("Trying with 9600");
    if(debugMode) Serial.print(gsmCmd[11]);
    mySerial.print(gsmCmd[11]);

    if(gsmResponse("OK", 2)){
      mySerial.end();
      delay(100);
      mySerial.begin(4800);
      delay(100);
      if(debugMode) Serial.println("Baud changed to 4800");
      gsmCmdResponse = true;
    }
    else{
      mySerial.end();
      delay(100);
      mySerial.begin(4800);
      delay(100);
      if(debugMode) Serial.println("Trying with 4800");
      if(debugMode) Serial.print(gsmCmd[11]);
      mySerial.print(gsmCmd[11]);
      
      if(gsmResponse("OK", 2)){
        if(debugMode) Serial.println("Baud changed to 4800");
        gsmCmdResponse = true;
      }
      else gsmCmdResponse = false;
    }
  }

  if(gsmCmdResponse){
    return true;
  }
  else return false;
}

char publishRequestToBroker(unsigned long UId, unsigned long ts){
  
  root["UID"] = UId;
  root["Time"] = ts;
  root["DeviceID"] = devID;
  root.printTo(myString);
  if(debugMode){
    Serial.println("Request json:");
    Serial.println(myString);
  }

  if(debugMode) Serial.print(gsmCmd[0]);
  mySerial.print(gsmCmd[0]);
    if(gsmResponse("OK", 5)){
      if(debugMode) Serial.print(gsmCmd[3]);
      mySerial.print(gsmCmd[3]);
      if(gsmResponse("OK", 10)){
        if(gsmResponse("CONNECT OK", 10)){
            if(debugMode) Serial.print(gsmCmd[4]);
            mySerial.print(gsmCmd[4]);
            if(gsmResponse(">", 10)){
              delay(50);
              connectToBroker();
              subscribeToBroker(subscribeTopic);
              sendDataToBroker(requestTopic, myString);
              sendTerminator();

              if(gsmResponse("SEND OK", 10)){
                myString = "\0";
              }
              else{
                myString = "\0";
                return 0;
              }
            }
            else{
              myString = "\0";
              return 0;
            }
        }
        else{
          if(debugMode) Serial.print(gsmCmd[10]);
          mySerial.print(gsmCmd[10]);
          if(gsmResponse("SHUT OK", 5)){
            if(checkGprs()){
              myString = "\0";
              return 4;
            }
            else{
              if(setGprs()){
                if(checkGprs()){
                  myString = "\0";
                  return 4;
                }
                else{
                  myString = "\0";
                  return 4;
                }
              }
              else{
                myString = "\0";
                return 4;
              }
            }
          }
          else{
            myString = "\0";
            return 0;
          }
        } 
      }
      else{
        if(debugMode) Serial.print(gsmCmd[10]);
        mySerial.print(gsmCmd[10]);
          if(gsmResponse("SHUT OK", 5)){
            if(checkGprs()){
              myString = "\0";
              return 4;
            }
            else{
              if(setGprs()){
                if(checkGprs()){
                  myString = "\0";
                  return 4;
                }
                else{
                  myString = "\0";
                  return 4;
                }
              }
              else{
                myString = "\0";
                return 4;
              }
            }
          }
          else{
            myString = "\0";
            return 0;
          }
      }
    }
    else{
      myString = "\0";
      return 0;
    }
    
    unsigned long previousTime = millis();
    receivedJsonIndex = 0;
    identifierCounter = 0;
    
    while((!responseFromBroker) && ((millis() - previousTime) < BROKER_RESPONSE_TIME)){
      
      if(mySerial.available() > 0){
        receivedByte = mySerial.read();
        if(debugMode) Serial.print(receivedByte);

        if(receivedByte == '{'){
          identifierCounter = 1;
        }
        else if(receivedByte == '}'){
        identifierCounter = 2;
        }

        if(identifierCounter == 1){
          if(receivedByte > 31 && receivedByte < 127){
            receivedJson[receivedJsonIndex] = receivedByte;
            receivedJsonIndex += 1;
          }
          
        }
        else if(identifierCounter == 2){
          receivedJson[receivedJsonIndex] = receivedByte;
          receivedJson[receivedJsonIndex + 1] = '\0';
          receivedJsonIndex = 0;
          identifierCounter = 0;
          if(debugMode){
            Serial.println();
            Serial.print("Received from broker: ");
            Serial.println(receivedJson);
          }

          mySerial.print(gsmCmd[5]);
          gsmResponse("CLOSE OK", 10);

          StaticJsonBuffer<512> jsonBuffer;
          JsonObject& jsonResponse = jsonBuffer.parseObject(receivedJson);

          if(jsonResponse["StatusCode"] == 2){
            jsonToPrint = "\0";
            jsonResponse["Message"].printTo(jsonToPrint);
            brokerResponseMessage = jsonToPrint;
            if(debugMode){
              Serial.println(brokerResponseMessage);
              Serial.println(jsonToPrint);
            }
            jsonToPrint = "\0";
            return 2;
          }
          else if(jsonResponse["StatusCode"] == 1){
            jsonToPrint = "\0";
            if(debugMode) Serial.print("Remaining water balance: ");
            jsonResponse["Water_Balance"].printTo(jsonToPrint);
            if(debugMode) Serial.println(jsonToPrint + " Litre");
            balanceInfo = jsonToPrint.toFloat();
            if(debugMode) Serial.println(balanceInfo);
            jsonToPrint = "\0";
            jsonResponse["Calibration"].printTo(jsonToPrint);
            calibrationInfo = jsonToPrint.toFloat();
            jsonToPrint = "\0";
            if(debugMode) Serial.print("Remaining balance: ");
            jsonResponse["Balance"].printTo(jsonToPrint);
            if(debugMode) Serial.println(jsonToPrint + " Tk");
            jsonToPrint = "\0";
            return 1;
          }

          jsonBuffer.clear();
          responseFromBroker = true;
      }
    }
    else{
      __asm__("nop");
    }
  }

  if(!responseFromBroker){
    myString = "\0";
    if(debugMode){
      Serial.println("No response from broker!");
      Serial.print(gsmCmd[5]);
    }
    mySerial.print(gsmCmd[5]);
    gsmResponse("CLOSE OK", 10);
    return 3;
  }
}

char publishBalanceToBroker(unsigned long UId, float waterDispensed, unsigned long ts){
  rootUpdate["UID"] = UId;
  rootUpdate["Time"] = ts;
  rootUpdate["DeviceID"] = devID;
  rootUpdate["Dispensed"] = waterDispensed;
  rootUpdate.printTo(myString);
  if(debugMode) Serial.println(myString);

  if(debugMode) Serial.print(gsmCmd[0]);
  mySerial.print(gsmCmd[0]);
    if(gsmResponse("OK", 5)){
      if(debugMode) Serial.print(gsmCmd[3]);
      mySerial.print(gsmCmd[3]);
      if(gsmResponse("OK", 10)){
        if(gsmResponse("CONNECT OK", 10)){
            if(debugMode) Serial.print(gsmCmd[4]);
            mySerial.print(gsmCmd[4]);
            if(gsmResponse(">", 10)){
              connectToBroker();
              subscribeToBroker(subscribeTopic);
              sendDataToBroker(dispenseTopic, myString);
              sendTerminator();

              if(gsmResponse("SEND OK", 10)){
                myString = "\0";
                delay(500);
                if(debugMode) Serial.print(gsmCmd[5]);
                mySerial.print(gsmCmd[5]);
                gsmResponse("CLOSE OK", 10);
                return 1;
              }
              else{
                myString = "\0";
                return 0;
              }
            }
            else{
              myString = "\0";
              return 0;
            }
        }
        else{
          if(debugMode) Serial.print(gsmCmd[10]);
          mySerial.print(gsmCmd[10]);
          if(gsmResponse("SHUT OK", 5)){
            if(checkGprs()){
              myString = "\0";
              return 4;
            }
            else{
              if(setGprs()){
                if(checkGprs()){
                  myString = "\0";
                  return 4;
                }
                else{
                  myString = "\0";
                  return 4;
                }
              }
              else{
                myString = "\0";
                return 4;
              }
            }
          }
          else{
            myString = "\0";
            return 0;
          }
        } 
      }
      else{
        if(debugMode) Serial.print(gsmCmd[10]);
        mySerial.print(gsmCmd[10]);
          if(gsmResponse("SHUT OK", 5)){
            if(checkGprs()){
              myString = "\0";
              return 4;
            }
            else{
              if(setGprs()){
                if(checkGprs()){
                  myString = "\0";
                  return 4;
                }
                else{
                  myString = "\0";
                  return 4;
                }
              }
              else{
                myString = "\0";
                return 4;
              }
            }
          }
          else{
            myString = "\0";
            return 0;
          }
      }
    }
    else{
      myString = "\0";
      return 0;
    }
}

void connectToBroker(){
  mySerial.write(0x10);
  mySerial.write(0x0C);
    
  mySerial.write((byte)0x00);
  mySerial.write(0x04);
  mySerial.write(0x4D);
  mySerial.write(0x51);
  mySerial.write(0x54);
  mySerial.write(0x54);
    
  mySerial.write(0x04);
    
  mySerial.write(0x02);

  mySerial.write((byte)0x00);
  mySerial.write(0x0F);

  mySerial.write((byte)0x00);
  mySerial.write((byte)0x00);
}

void subscribeToBroker(String topic){
  int topicLength = topic.length();
  int counter = 0;
  int lengthOfVariableHeader = topicLength + 2 + 2 + 1;
  
  mySerial.write(0x82);
  mySerial.write(lengthOfVariableHeader);

  mySerial.write((byte)0x00);
  mySerial.write(0x0A);

  if(topicLength < 255){
    mySerial.write((byte)0x00);
    mySerial.write(topicLength);
  }
  else{
    mySerial.write((topicLength >> 8));
    mySerial.write(topicLength & 0xFF);
  }

  for(counter = 0; counter < topicLength; counter++){
    mySerial.write(topic[counter]);
  }

  mySerial.write((byte)0x00);
}

void sendDataToBroker(String topic, String payload){
  int topicLength = topic.length();
  int payloadLength = payload.length();
  int counter = 0;
  int totalLenghtOfVariablePart = topicLength + payloadLength + 2;
  
  mySerial.write(0x30);
  mySerial.write(totalLenghtOfVariablePart);

  if(topicLength < 255){
    mySerial.write((byte)0x00);
    mySerial.write((topicLength));
  }
  else{
    mySerial.write((topicLength >> 8));
    mySerial.write((topicLength & 0xFF));
  }

  for(counter = 0; counter < topicLength; counter++){
    mySerial.write(topic[counter]);
  }

  for(counter = 0; counter < payloadLength; counter++){
    mySerial.write(payload[counter]);
  }
}

void sendTerminator(){
  mySerial.write(0x1A);
}

bool gsmResponse(String expectedResponse, unsigned long int waitTime){
  
  char serialBuffer[512], receivedChar = '\0';
  int serialBufferCounter = 0, newLineCounter = 0;
  String receivedResponse;
  waitTime *= 1000;
  bool responseStatus = false, validResponse = false;
  bool readyToSend = false;

  serialBuffer[0] = '\0';
  
  unsigned long previousTime = 0;
  
  previousTime = millis();

  if(expectedResponse == ">"){
    while((millis() - previousTime) < waitTime){
     if(mySerial.available() > 0){
      receivedChar = mySerial.read();
      //if(debugMode) Serial.print(receivedChar);

      if(receivedChar == '>'){
        responseStatus = true;
        delay(250);
        return true;
      }
    }
   }
  if(!responseStatus){
    return false; 
  }
  }

  if(expectedResponse == CONNACK || expectedResponse == SUBACK){
    while((millis() - previousTime) < waitTime){
     if(mySerial.available() > 0){
      receivedChar = mySerial.read();
      //if(debugMode) Serial.print(receivedChar);

      if(receivedChar == byte(0x00)){
        ackCounter += 1;
      }
      if(ackCounter == 2){
        delay(250);
        responseStatus = true;
        ackCounter = 0;
        return true;
      }
    }
   }
  if(!responseStatus){
    return false; 
  }    
  }
  
  else{
     while((millis() - previousTime) < waitTime){
     if(mySerial.available() > 0){
      receivedChar = mySerial.read();
      if(debugMode) Serial.print(receivedChar);
      
      if(networkStrenghtRead){
        
        if(receivedChar == ','){
          storeStrengthValue = false;
          strengthReadComplete = true;
        }
        
        if(storeStrengthValue){
          strengthString += receivedChar;
        }
        
        if(receivedChar == ' '){
          storeStrengthValue = true;
        }

        if(strengthReadComplete){
        strengthValue = strengthString.toInt();
        networkStrenghtRead = false;
        }
      }

     if((receivedChar == '\r') || (receivedChar == '\n')){
       serialBuffer[serialBufferCounter] = '\0';

       if (receivedChar == '\n'){
         responseStatus = true;
        
         if(serialBuffer[0] == '\0'){
          validResponse = false;
         }
         else validResponse = true;
        
         if(validResponse){
           receivedResponse = serialBuffer;
        
           for(serialBufferCounter = 0; serialBufferCounter < 512; serialBufferCounter++){
             serialBuffer[serialBufferCounter] = '\0';
           }
           serialBufferCounter = 0;

           if(receivedResponse == expectedResponse){
             //Serial.println("Got correct response");
             //delay(300);
             return true;
           }
           else{
             //Serial.println("Got incorrect response");
             return false;
           }
         }
       }
     }
     else{
      //if(receivedChar > 31 && receivedChar < 127){
        serialBuffer[serialBufferCounter] = receivedChar;
        if(serialBuffer[serialBufferCounter] == expectedResponse[serialBufferCounter]){
          serialBufferCounter += 1;
        }
        
      //}
     }
    }
   }
    if(!responseStatus){
     //Serial.println("Response timeout!");
     return false;
   }
  }
}
