#include "led.h"
#include <Arduino.h>

const int ledPin = 7;  // LED connected to pin 7
const int LED = 6;

void led_setup(void) {
    pinMode(ledPin, OUTPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(ledPin, LOW);  // Initialize LED as off
    digitalWrite(LED, HIGH);  
}

void led_on(void) {
    digitalWrite(ledPin, HIGH);  // Turn on LED
    digitalWrite(LED,LOW);
}

void led_off(void) {
    digitalWrite(ledPin, LOW);  // Turn off LED
    digitalWrite(LED, HIGH);
}
