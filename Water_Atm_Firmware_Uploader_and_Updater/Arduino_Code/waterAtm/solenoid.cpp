#include "solenoid.h"

void initiateSolenoid(){
  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, LOW);
}

void openSolenoid(){
  digitalWrite(SOLENOID_PIN, HIGH);
}

void closeSolenoid(){
  digitalWrite(SOLENOID_PIN, LOW);
}
