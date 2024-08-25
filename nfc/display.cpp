
#include "display.h"
// #include "global_variables.h"

LiquidCrystal_I2C lcd(0x27,20,4); 

void display_init(void)
{
  lcd.init();
  
  lcd.backlight();
  lcd.clear();
}

void display_test(void)
{
  lcd.setCursor(2,0);
  lcd.print("Display    test");
}

void display_card1(void)
{

  lcd.setCursor(2,0);
  lcd.print("Card 1 is found");

}

void display_card2(void)
{
  lcd.setCursor(2,0);
  lcd.print("Card 2 is found");

}