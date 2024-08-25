#include <ArduinoModbus.h>

// Define Modbus parameters
#define RS485_DE_PIN 2       // Enable pin for RS485 (connected to 5V or GND)
#define SLAVE_ID 1            // ID of this slave Arduino

// Define a variable to hold the register value
uint16_t registerValue = 0; // Example: Initialize with a value

// Create Modbus RTU slave instance
ModbusSlave slave(Serial1); // Assuming software serial on pin 8

void setup() {
  // Initialize serial communication for Modbus RTU
  Serial1.begin(9600);

  // Set the slave ID
  slave.setID(SLAVE_ID);

  //
