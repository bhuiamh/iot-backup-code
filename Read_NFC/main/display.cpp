#include "display.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

void display_init(void) {
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void display_show_card_info(String cardUID) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card UID:");
    lcd.setCursor(0, 1);
    lcd.print(cardUID);
}
void display_card_not_present(){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Welcome !");
    lcd.setCursor(0, 1);
    lcd.print("Card Not Present");
    lcd.setCursor(0, 3);
    lcd.print("Please Insert a Card");
}
