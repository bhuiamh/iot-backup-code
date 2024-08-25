#include "uv.h"

void initiateUv(){
  pinMode(UV_PIN, OUTPUT);
  digitalWrite(UV_PIN, LOW);
}

void openUv(){
  digitalWrite(UV_PIN, HIGH);
}

void closeUv(){
  digitalWrite(UV_PIN, LOW);
}
