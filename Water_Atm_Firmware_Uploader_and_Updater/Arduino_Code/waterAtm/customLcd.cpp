#include "customLcd.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_ROWS, LCD_COLUMNS);

uint8_t lcdPosition = 0;  // variable for incrementing lcd cursor position during ID printing
uint8_t lcdLineNumber = 0;
String inputMessage;

void initiateLcd(){
  lcd.init();
  lcd.backlight();
}

void printCardIdOnLcd(byte * id){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Card found!");
  lcd.setCursor(0,1);
  lcd.print("ID:");
  for(byte i = 0; i < CARD_UID_LENGTH; i++){
    lcdPosition += 1;
    lcd.setCursor((3 + i + lcdPosition), 1);
    if(*(id + i) < 0x10)
      lcd.print("0");
    lcd.print(*(id + i), HEX);
  }
  lcdPosition = 0;
}

void printCardRemovalMessage(float amount, float cost, float balance){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Balance:");
  lcd.setCursor(9,0);
  lcd.print(balance);
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

void printWaterStatus(float amount){
  lcd.clear();
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

void printBootMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Water ATM V3.0");
  lcd.setCursor(0,1);
  lcd.print(" Powered by: NybSys");
  lcd.setCursor(0,3);
  lcd.print("System is booting...");
}

void printGsmRebootMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" GSM is rebooting!");
  lcd.setCursor(0,1);
  lcd.print("    Please wait!");
}

void printNetworkError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Cannot establish");
  lcd.setCursor(0,1);
  lcd.print(" GSM communication!");
}

void printEthernetPresentMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Accessing server");
  lcd.setCursor(0,1);
  lcd.print("  through Ethernet!");
}

void printEthernetAbsentMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Ethernet is not");
  lcd.setCursor(0,1);
  lcd.print("      present!");
  lcd.setCursor(0,2);
  lcd.print("  Accessing server");
  lcd.setCursor(0,3);
  lcd.print("    through GSM!");
}

void printRfidReaderError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  RFID Card Reader");
  lcd.setCursor(0, 1);
  lcd.print("   is not working!");
}

void printSimPresenceMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  SIM card present");
}

void printSimAbsenceMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" SIM card not found");
}

void printNetworkOkMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Network found");  
}

void printNetworkFailMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SIM is not registere");
  lcd.setCursor(0,1);
  lcd.print("d to network");  
}

void printNetworkStrengthNone(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Network Strength:");
  lcd.setCursor(0,1);
  lcd.print("None");  
}

void printNetworkStrengthLow(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Network Strength:");
  lcd.setCursor(0,1);
  lcd.print("Poor");  
}

void printNetworkStrengthMed(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Network Strength:");
  lcd.setCursor(0,1);
  lcd.print("Medium");  
}

void printNetworkStrengthHigh(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Network Strength:");
  lcd.setCursor(0,1);
  lcd.print("High");
}

void printNetworkStrengthUnknown(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Network Strength:");
  lcd.setCursor(0,1);
  lcd.print("Unknown");
}

void printGprsSucessMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GPRS enabled");
}

void printGprsFailMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Can not establish");  
  lcd.setCursor(0,1);
  lcd.print("GPRS connection");
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

void printClockFailMessageWithRetry(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RTC configuration:");
  lcd.setCursor(0,1);
  lcd.print("Failed");
  lcd.setCursor(0,2);
  lcd.print("Retrying..."); 
}

void printGsmModuleError(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM module error");
}

void printGsmErrorZero(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 0");
}

void printGsmErrorOne(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 1");
}

void printGsmErrorTwo(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 2");
}

void printGsmErrorThree(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 3");
}

void printGsmErrorFour(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 4");
}

void printGsmErrorFive(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 5");
}

void printGsmErrorSix(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 6");
}

void printGsmErrorSeven(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 7");
}

void printGsmErrorEight(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM Error: 8");
}

void printBrokerNotRespondingMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("No response from");
  lcd.setCursor(0,1);
  lcd.print("broker");  
}

void printGsmRebootedMessage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GSM rebooted");
  lcd.setCursor(0,1);
  lcd.print("Please try again");  
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
