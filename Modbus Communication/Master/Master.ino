#include <SoftwareSerial.h>
#include <ModbusRTUMaster.h>

const uint8_t rxPin = 10;
const uint8_t txPin = 11;
const uint8_t dePin = 13;

SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUMaster modbus(mySerial, dePin); // Serial port, driver enable pin for RS-485

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(dePin, OUTPUT);

  mySerial.begin(9600); // Baud rate
  modbus.begin(9600); // Modbus baud rate

  Serial.begin(9600); // For debugging
}

void loop() {
  const char *message1 = "Hello, Slave 1!";
  const char *message2 = "Hello, Slave 2!";
  uint16_t messageBuffer[32]; // Buffer to hold the message

  // Send message to Slave 1
  sendMessageToSlave(1, message1, messageBuffer);

  delay(1000); // Wait for 1 second

  // Send message to Slave 2
  sendMessageToSlave(2, message2, messageBuffer);

  delay(1000); // Wait for 1 second
}

void sendMessageToSlave(uint8_t slaveId, const char *message, uint16_t *messageBuffer) {
  uint16_t messageLength = strlen(message);

  // Convert the message to uint16_t array
  for (uint16_t i = 0; i < messageLength; i++) {
    messageBuffer[i] = message[i];
  }

  // Send the message to the specified slave
  if (modbus.writeMultipleHoldingRegisters(slaveId, 0, messageBuffer, messageLength) == 0) {
    Serial.print("Message sent to Slave ");
    Serial.println(slaveId);
  } else {
    Serial.print("Failed to send message to Slave ");
    Serial.println(slaveId);
  }
}
