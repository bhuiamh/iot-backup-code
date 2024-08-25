#include <Arduino.h>

const int vibrationPin = D1; // Replace 2 with your chosen digital pin

void setup() {
  pinMode(vibrationPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorState = digitalRead(vibrationPin);
  if (sensorState == HIGH) {
    Serial.println("Vibration detected");
  } else {
    Serial.println("No vibration");
  }
  delay(100); // Adjust delay as needed
}
