#include "customLcd.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_ROWS, LCD_COLUMNS);

uint8_t lcdPosition = 0;  // variable for incrementing lcd cursor position during ID printing
uint8_t lcdLineNumber = 0;
String inputMessage;

void initiateLcd(){
  lcd.init();
  lcd.backlight();
}

// void printCardIdOnLcd(byte * id){
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print("Card found!");
//   lcd.setCursor(0,1);
//   lcd.print("ID:");
//   for(byte i = 0; i < CARD_UID_LENGTH; i++){
//     lcdPosition += 1;
//     lcd.setCursor((3 + i + lcdPosition), 1);
//     if(*(id + i) < 0x10)
//       lcd.print("0");
//     lcd.print(*(id + i), HEX);
//   }
//   lcdPosition = 0;
// }
void printcard( unsigned long int cardIDinDec)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(cardIDinDec);
}

void printCardRemovalMessage(float amount, float cost, float balance){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Balance:");
  lcd.setCursor(9,0);
  lcd.print(balance - amount);
  lcd.setCursor(16,0);
  lcd.print("L");
  lcd.setCursor(0,1);
  lcd.print("Dispensed:");
  lcd.setCursor(11,1);
  lcd.print(amount);
  lcd.setCursor(17,1);
  lcd.print('L');
//  lcd.setCursor(0,2);
//  lcd.print("Cost:");
//  lcd.setCursor(6,2);
//  lcd.print(cost);
//  lcd.setCursor(11,2);
//  lcd.print("Tk");
}

void printWelcomeMessage(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("  No card present!");
  lcd.setCursor(0,2);
  lcd.print("Please place a card!");
}

void printWaitMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Please wait...");
}

void printWaterStatus(float amount, float cardBalance){

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Balance:");
  lcd.setCursor(11, 0);
  lcd.print(cardBalance);
  lcd.setCursor(0,1);
  lcd.print("Dispensed:");
  lcd.setCursor(11,1);
  lcd.print(amount);
  lcd.setCursor(16,1);
  lcd.print('L'); 
}

void printBalanceError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Insufficient Balance");
}

void printAuthenticationError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Unauthorized Card!");
}

void printConnectionError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Please try again!");
}

void printNetError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("No Internet");
  lcd.setCursor(0, 1);
  lcd.print(" Please try later");
}



void printRfidReaderError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  RFID Card Reader");
  lcd.setCursor(0, 1);
  lcd.print("   is not working!");
}



void printClockSyncingMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Setting up RTC");
  lcd.setCursor(0,1);
  lcd.print("   Please wait...");
}

void printClockSucessMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RTC configuration:");
  lcd.setCursor(0,1);
  lcd.print("Successful");  
}

void printClockFailMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RTC configuration:");
  lcd.setCursor(0,1);
  lcd.print("Failed");  
}




void printBrokerNotRespondingMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("No response from");
  lcd.setCursor(0,1);
  lcd.print("broker");  
}



void showErrorMessage(char msg[]){
  lcd.clear();
  lcdLineNumber = 0;
  
  char * token = strtok(msg, "|");

  while(token != NULL){
    lcd.setCursor(0,lcdLineNumber);
    lcd.print(token);
    lcdLineNumber += 1;
    token = strtok(NULL, "|"); 
  }
}

void printToLCD(String message) {
  lcd.clear();  // Clear the LCD screen

  int maxCharsPerLine = 20;  // Maximum characters per line
  int lineNumber = 0;

  // Print the message line by line
  while (message.length() > 0 && lineNumber < 4) {
    String line = message.substring(0, maxCharsPerLine);  // Get the first 20 characters
    lcd.setCursor(0, lineNumber);  // Set cursor to the beginning of the line
    lcd.print(line);  // Print the line to the LCD
    message = message.substring(maxCharsPerLine);  // Remove the printed part from the message
    lineNumber++;  // Move to the next line
  }
}
