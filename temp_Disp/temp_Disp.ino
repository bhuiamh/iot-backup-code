#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Address 0x27 for 16x2 LCD

void setup() {
  sensors.begin();
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
}

void loop() {
  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);
  float tempF = (tempC * 9.0) / 5.0 + 32.0;

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Temperature:");
  lcd.setCursor(3, 1);
  lcd.print(tempC);
  lcd.print("C  ");
  lcd.print(tempF);
  lcd.print("F");

  float tempC2 = sensors.getTempCByIndex(1);z
  float tempF2 = (tempC2 * 9.0) / 5.0 + 32.0;
  lcd.setCursor(2, 2);
  lcd.print("Temperature 2:");
  lcd.setCursor(3, 3);
  lcd.print(tempC2);
  lcd.print("C  ");
  lcd.print(tempF2);
  lcd.print("F");

  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" Done By: (GROUP E)");
  lcd.setCursor(0, 2);
  lcd.print(" MAHMUDUL & RIZWAN");

  // Serial.print("Temperature: ");
  // Serial.print(tempC);
  // Serial.print(" C | ");
  // Serial.print(tempF);
  // Serial.println(" F");

  delay(1000);
}
