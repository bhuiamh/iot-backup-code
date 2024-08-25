#include "led.h"

#define led 13

void led_setup(void)
{
  pinMode(led,OUTPUT);
}

void led_on(void)
{
  digitalWrite(led,HIGH);
  
}

void led_off(void)
{
  digitalWrite(led,LOW);
  
}