#include <SoftwareSerial.h>

#define DE_RE 8  // Control pin for direction (HIGH for transmit, LOW for receive)
#define RX 2     // Define RX pin for SoftwareSerial
#define TX 3     // Define TX pin for SoftwareSerial

String sendMessage;
String receivedMessage;

SoftwareSerial mySerial(RX, TX);

void setup() {
  Serial.begin(9600);    // Initialize the Serial monitor for debugging
  mySerial.begin(9600);  // Initialize SoftwareSerial for sending data

  pinMode(DE_RE, OUTPUT);
  digitalWrite(DE_RE, LOW);  // Set to receive mode
}

void loop() {
  // Receiving Mode
  digitalWrite(DE_RE, LOW);  // Enable reception
  if (mySerial.available() > 0) {
    char receivedChar = mySerial.read();
    if (receivedChar == '\n') {
      Serial.print("Received: ");
      Serial.println(receivedMessage);  // Print the received message in the Serial monitor

      delay(100);  // Short delay to ensure message reception

      // Switch to sending mode and send acknowledgment
      digitalWrite(DE_RE, HIGH);  // Enable sending
      delay(10);  // Short delay to ensure mode switch
      mySerial.println("Recieved The Message");
      delay(100);  // Short delay to ensure message is sent

      receivedMessage = "";  // Reset the received message
      digitalWrite(DE_RE, LOW);  // Switch back to receive mode
    } else {
      receivedMessage += receivedChar;  // Append characters to the received message
    }
  }

  // Sending Mode
  if (Serial.available() > 0) {
    char inputChar = Serial.read();
    if (inputChar == '\n') {
      digitalWrite(DE_RE, HIGH);      // Enable sending
      mySerial.println(sendMessage);  // Send the message through SoftwareSerial with a newline character
      Serial.print("Sending: ");
      Serial.println(sendMessage);
      sendMessage = "";          // Reset the message
      delay(100);                // Short delay to allow message to be sent before switching to receive mode
      digitalWrite(DE_RE, LOW);  // Switch back to reception mode
    } else {
      sendMessage += inputChar;  // Append characters to the message
    }
  }
}
