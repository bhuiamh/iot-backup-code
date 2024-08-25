#include "nfc_card.h"
#include "display.h"
#include "led.h"
#include "global_variables.h"

void setup() {
    Serial.begin(9600);
    nfc_init();
    display_init();
    led_setup();
    led_off();
}

void loop() {
    Serial.println("Scan an NFC tag");

    

    nfc_read();  // Read the NFC tag and update the global variables

    if (card_presented) {
        Serial.println("Card UID: " + cardUID);  // Print the UID to the Serial Monitor
        display_show_card_info(cardUID);         // Display the UID on the LCD
        led_on();                                // Turn on the LED
    } else {
        display_card_not_present();         // Display the UID on the LCD
        led_off();  // Turn off the LED
    }
    
}