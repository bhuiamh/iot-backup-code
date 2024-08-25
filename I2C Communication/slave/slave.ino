#include <Wire.h>

int LED = 8;
int transAddress = 111;
bool messageReceived = false;

void setup() {
  Serial.begin(9600);     // Initialize Serial communication
  pinMode(LED, OUTPUT);   // Define the LED pin as Output
  Wire.begin(transAddress);// Start the I2C Bus as Slave on address 11
  Wire.onReceive(receiveEvent); // Attach a function to trigger when something is received
  Wire.onRequest(requestEvent);
}

void receiveEvent() {
  while (Wire.available()) { 
    char c = Wire.read(); // Read a byte
    Serial.print(c);      // Print the received byte
  }
  Serial.println();
  messageReceived = true; // Set flag to indicate a message was received
}

void loop() {
  if (messageReceived) {
    messageReceived = false; // Reset flag
    digitalWrite(LED, HIGH); // Turn on LED
    digitalWrite(7, LOW);    // Is 7 a typo? Should it be LED instead?
    delay(2000);             // Wait for 2 seconds
    digitalWrite(LED, LOW);  // Turn off LED
    digitalWrite(7, HIGH);   // Again, should it be LED instead?
  }
}


void requestEvent(){
  Wire.write("1");
}
