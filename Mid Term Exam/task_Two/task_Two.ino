#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

// Define pins and constants
#define DE_RE 2  // Control pin for direction (HIGH for transmit, LOW for receive)
#define RX 6     // RX pin for SoftwareSerial
#define TX 7     // TX pin for SoftwareSerial

// Create instances of SoftwareSerial and LiquidCrystal_I2C
SoftwareSerial mySerial(RX, TX);  
LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD with I2C address 0x27 and size 20x4

// Variables to hold messages
String sendMessage;
String receivedMessage;

void setup() {
  // Initialize Serial and SoftwareSerial
  Serial.begin(9600);         // Serial monitor for debugging
  mySerial.begin(9600);       // SoftwareSerial for communication

  // Initialize LCD
  lcd.init();                 // Initialize the LCD
  lcd.backlight();           // Turn on the LCD backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System is ready");

  // Set DE_RE pin mode
  pinMode(DE_RE, OUTPUT);
  digitalWrite(DE_RE, LOW);  // Set to receive mode initially
}

void loop() {
  // Receiving Mode
  digitalWrite(DE_RE, LOW);  // Enable reception mode
  while (mySerial.available() > 0) {
    char receivedChar = mySerial.read(); // Read incoming character
    
    // Only append printable characters
    if (isPrintable(receivedChar)) {
      receivedMessage += receivedChar;  // Append character to message
    }
    
    if (receivedChar == '\n') {
      // End of message
      receivedMessage.trim();  // Trim any whitespace characters

      // Display the received message
      Serial.println(receivedMessage);  // Print the received message to Serial monitor
      mySerial.println("Received The Message");  // Send acknowledgment
      
      // Print received message to LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Message from Rizwan:");
      lcd.setCursor(0, 1);
      lcd.print(receivedMessage);
      receivedMessage = "";  // Reset received message for next input
    }
  }

  // Sending Mode
  if (Serial.available() > 0) {
    char inputChar = Serial.read();  // Read character from Serial monitor
    
    // Only append printable characters
    if (isPrintable(inputChar)) {
      sendMessage += inputChar;  // Append character to message
    }
    
    if (inputChar == '\n') {
      // End of input message
      digitalWrite(DE_RE, HIGH);  // Enable transmission mode
      mySerial.println(sendMessage);  // Send the message through SoftwareSerial
      Serial.print("Sending: ");
      Serial.println(sendMessage);
      
      // Print sending message to LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Message from me:");
      lcd.setCursor(0, 1);
      lcd.print(sendMessage);
      sendMessage = "";  // Reset message for next input
      delay(100);  // Short delay to ensure message is sent before switching back to receive mode
      digitalWrite(DE_RE, LOW);  // Switch back to receive mode
    }
  }
}
