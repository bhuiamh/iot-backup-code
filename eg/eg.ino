#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD with I2C address 0x27 and size 20x4

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System is ready");
  delay(2000);  // Wait 2 seconds before starting the countdown
}

void loop() {
  // Countdown from 5 to 1 in the middle of the screen
  for (int i = 5; i > 0; i--) {
    lcd.clear();
    lcd.setCursor(9, 1);  // Center position for 20x4 LCD
    lcd.print(i);
    delay(1000);  // 1-second delay between each number
  }

  // Clear the screen after the countdown
  lcd.clear();

  // Display "Welcome to"
  lcd.setCursor(4, 1);  // Adjust the position as needed
  lcd.print("Welcome to");

  // Animate "Electrical Ghost" appearing one by one
  String text = "Electrical Ghost";
  lcd.setCursor(2, 2);  // Adjust the position as needed

  for (int i = 0; i < text.length(); i++) {
    lcd.print(text[i]);  // Print each character one by one
    delay(100);          // Delay between each letter for the animation effect
  }

  delay(2000);  // Wait before repeating the sequence
}
