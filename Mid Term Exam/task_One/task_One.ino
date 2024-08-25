#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions for LEDs
const int RED_LED = 8;
const int GREEN_LED = 9;
const int BLUE_LED = 10;

// Pin for the OneWire bus
#define ONE_WIRE_BUS 4
#define SENSOR_INDEX 0 // Index of the temperature sensor

// Set up OneWire communication with the DS18B20 sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Initialize the LCD with I2C address 0x27 for a 20x4 display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Start communication with the temperature sensor
  sensors.begin();
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set up the LED pins as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // Initialize the LCD and turn on the backlight
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Request temperature from the sensor
  sensors.requestTemperatures();
  
  // Get the temperature in Celsius
  float tempC = sensors.getTempCByIndex(SENSOR_INDEX);
  
  // Basic error checking for sensor reading
  if (tempC < -55 || tempC > 125) {
    Serial.println("Invalid temperature reading");
    return;
  }

// Check the temperature and turn on the appropriate LED
if (tempC <= 26) {
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
} 
else if (tempC > 26 && tempC < 28) {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  
} 
else if (tempC >= 28) {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

  // Convert the temperature to Fahrenheit
  float tempF = (tempC * 9.0) / 5.0 + 32.0;

  // Display temperature on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Room Temperature: ");
  lcd.setCursor(6, 1);
  lcd.print(tempC, 1); // Display temperature with one decimal place
  lcd.print(" C");
  lcd.setCursor(6, 2);
  lcd.print(tempF, 1); // Display temperature with one decimal place
  lcd.print(" F");

  Serial.print("Room Temperature: ");
  Serial.print(tempC); 
  Serial.print(" C  || ");
  Serial.print(tempF); 
  Serial.println(" F");



  
  // Wait 2 seconds before updating the display again
  delay(200);
}
