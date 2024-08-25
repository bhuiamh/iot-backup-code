#include "nfc_card.h"
#include "display.h"
#include "led.h"

uint8_t card_serial=0;

void setup() {
    Serial.begin(9600);
    nfc_init();
    display_init();
    display_test();
    led_setup();
    led_off();
}
    
void loop() {
    Serial.println("Scan a NFC tag");
    nfc_read();
    card_serial=nfc_status_check();

    if (card_serial == 1)
    {
      display_card1();
      led_on();
      Serial.println(card_serial);

    }
    if (card_serial == 2)
    {
      display_card2();
      led_on();
    }
    
    led_off();
    //delay(50);
}