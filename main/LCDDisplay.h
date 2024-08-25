#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LCDDisplay {
public:
    LCDDisplay(uint8_t lcdAddr, uint8_t lcdCols, uint8_t lcdRows);
    void initialize();
    void printMessage(const char* message, uint8_t col, uint8_t row);
    void clear();

private:
    LiquidCrystal_I2C lcd;
};

#endif
