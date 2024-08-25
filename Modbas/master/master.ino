#include <ArduinoModbus.h>

// Define Modbus parameters
#define RS485_DE_PIN 2       // Enable pin for RS485 (connected to GND or 5V)
#define SLAVE_ID 1            // ID of the slave Arduino
#define REG_ADDRESS 0x00     // Register address to write

// Create Modbus RTU master instance
ModbusMaster master = ModbusMaster(Serial1); // Assuming software serial on pin 8

int receivedValue;

void setup() {
  // Initialize serial communication for Modbus RTU
  Serial1.begin(9600);

  // Set RS485 enable pin (optional, depending on your RS485 module)
  pinMode(RS485_DE_PIN, OUTPUT);
  digitalWrite(RS485_DE_PIN, LOW); // Enable RS485 transmission during communication

  // Initialize serial communication for reading data (optional)
  Serial.begin(9600);
}

void loop() {
  // Read data from serial (replace with your reading method)
  if (Serial.available()) {
    receivedValue = Serial.parseInt(); // Example: Read integer from serial
  }

  // Write the received value to the slave register
  master.writeHoldingRegisters(SLAVE_ID, REG_ADDRESS, 1, &receivedValue);

  // Process or display the received value (optional)
  Serial.print("Received value (Serial): ");
  Serial.println(receivedValue);

  delay(1000);
}
