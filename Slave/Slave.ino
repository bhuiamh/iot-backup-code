#if __AVR__
  #define USE_SOFTWARE_SERIAL
#endif

#if defined USE_SOFTWARE_SERIAL
  #include <SoftwareSerial.h>
#endif

#include <ModbusRTUSlave.h>

const byte rxPin = 10;
const byte txPin = 11;
const byte dePin = 13;

#if defined USE_SOFTWARE_SERIAL
  SoftwareSerial mySerial(rxPin, txPin);
  ModbusRTUSlave modbus(mySerial, dePin); // Serial port, driver enable pin for RS-485
#else
  #if (defined __AVR_ATmega328P__ || defined __AVR_ATmega168__ || defined __AVR_ATmega1280__ || defined __AVR_ATmega2560__)
    ModbusRTUSlave modbus(Serial, dePin); // Serial port, driver enable pin for RS-485
  #elif defined ESP32
    ModbusRTUSlave modbus(Serial0, dePin); // Serial port, driver enable pin for RS-485
  #else
    ModbusRTUSlave modbus(Serial1, dePin); // Serial port, driver enable pin for RS-485
  #endif
#endif

uint16_t messageBuffer[32]; // Buffer to hold the received message

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(dePin, OUTPUT);

  #if defined USE_SOFTWARE_SERIAL
    mySerial.begin(9600); // Baud rate
  #else
    Serial.begin(9600); // Baud rate for hardware serial
  #endif

  modbus.configureHoldingRegisters(messageBuffer, 32); // Configure holding registers
  modbus.begin(1, 9600); // Modbus ID and baud rate

  Serial.begin(9600); // For debugging
  Serial.println("Slave 1 started");
}

void loop() {
  modbus.poll(); // Poll Modbus

  // Print the received message to the Serial Monitor
  String receivedMessage;
  for (uint16_t i = 0; i < 32; i++) {
    if (messageBuffer[i] == 0) break;
    receivedMessage += (char)messageBuffer[i];
  }

  if (receivedMessage.length() > 0) {
    Serial.println("Received: " + receivedMessage); // Print received message to Serial Monitor
  }

  delay(1000);
}
