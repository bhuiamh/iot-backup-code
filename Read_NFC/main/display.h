#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

void display_init(void);
void display_show_card_info(String cardUID);
void display_card_not_present();         // Display the UID on the LCD



#endif
