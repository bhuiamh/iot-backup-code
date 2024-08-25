#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// MAC address for Ethernet shield
byte mac[] = { 0xDE, 0xBD, 0xBE, 0xEF, 0xFE, 0xED };

// Initialize the LCD with the I2C address and dimensions
LiquidCrystal_I2C lcd(0x27, 20, 4); // Address 0x27, 20 columns, 4 rows

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  // Initialize Ethernet with DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    lcd.setCursor(0, 1);
    lcd.print("Ethernet Failed");
    while (true);
  }
  delay(1000); // Give the Ethernet shield a second to initialize
  lcd.clear();
}

void loop() {
  // Check if Ethernet is connected by pinging a server
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");
    lcd.setCursor(0, 0);
    lcd.print("No Ethernet Shield");
  } else if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    lcd.setCursor(0, 0);
    lcd.print("Cable not conn.");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Ethernet OK");
    
    // Attempt to connect to a website (e.g., Google)
    if (connectToServer("www.google.com")) {
      Serial.println("Internet connected");
      lcd.setCursor(0, 1);
      lcd.print("Internet OK");
    } else {
      Serial.println("Internet not connected");
      lcd.setCursor(0, 1);
      lcd.print("No Internet");
    }
  }
  delay(5000); // Check every 5 seconds
}

bool connectToServer(const char* host) {
  EthernetClient client;
  if (client.connect(host, 80)) {
    client.stop();
    return true;
  }
  client.stop();
  return false;
}
