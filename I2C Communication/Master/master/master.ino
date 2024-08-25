#include <Wire.h>

String sendMessage = "";
int transAddress = 111;

void setup() {
  Wire.begin();         // Start the I2C Bus as Master
  Serial.begin(9600);   // Start the serial communication for receiving input
}

void loop() {
  if (Serial.available() > 0) {
    char inputChar = Serial.read();
    if (inputChar == '\n') {
      Wire.beginTransmission(transAddress);
      Wire.write(sendMessage.c_str());  // Send the actual message content
      Wire.endTransmission();
      sendMessage = "";  // Reset the message after sending

      Wire.requestFrom(transAddress, 1);
      while(Wire.available()){
        char achMess = Wire.read();
        // Serial.print(achMess);
         if (achMess == '1') {  // Compare with single quotes for character
          Serial.println("Message Received by Slave 1");
        } else if (achMess == '2') {
          Serial.println("Message Received by Slave 2");
        }
     
      }

    } else {
      sendMessage += inputChar;  // Append characters to the message
    }
  }
  
}
