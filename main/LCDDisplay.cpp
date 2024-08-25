#include "LCDDisplay.h"

LCDDisplay::LCDDisplay(uint8_t lcdAddr, uint8_t lcdCols, uint8_t lcdRows) 
    : lcd(lcdAddr, lcdCols, lcdRows) {}

void LCDDisplay::initialize() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");
}

void LCDDisplay::printMessage(const char* message, uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
    lcd.print(message);
}

void LCDDisplay::clear() {
    lcd.clear();
}
