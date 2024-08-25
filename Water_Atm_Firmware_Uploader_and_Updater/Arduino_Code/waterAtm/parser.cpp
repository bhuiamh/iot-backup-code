#include "parser.h"

char dataString[MAX_UART_DATA_LENGTH];
uint8_t stringIndex = 0;

void sendUidToBroker(byte * id){
  Serial.print('C');
  for(stringIndex = 0; stringIndex < CARD_UID_LENGTH; stringIndex++){
    if(*(id + stringIndex) < 0x10){
      Serial.print('0');
    }
    Serial.print(*(id + stringIndex), HEX);
    }
  Serial.println();
}

void sendCardBalance(float balance){
  Serial.print('B');
  Serial.print(balance);
  Serial.println();
}

void requestInternetStatus(){
  Serial.print('E');
  Serial.println();
}

void doParse(char dataString[]){
  switch(dataString[0]){
    case 'T':
    break;

    case 'C':
      switch(dataString[1]){
        case 'R':
          switch(dataString[2]){
            case 'Y':
              
            break;

            case 'N':
              
            break;
          }
        break;

        case 'I':
        break;
      }
    break;
  }
}
