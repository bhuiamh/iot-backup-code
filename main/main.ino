#include <SPI.h>
#include "EthernetModule.h"
#include "LCDDisplay.h"

// MAC address for Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetModule ethernetModule(mac);
LCDDisplay lcdDisplay(0x27, 20, 4);

void setup() {
  Serial.begin(9600);

  lcdDisplay.initialize();

  if (!ethernetModule.initialize()) {
    lcdDisplay.printMessage("Ethernet Failed", 0, 1);
    while (true);
  }
  lcdDisplay.clear();
}

void loop() {
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");
    lcdDisplay.printMessage("No Ethernet Shield", 0, 0);
  } else if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    lcdDisplay.printMessage("Cable not conn.", 0, 0);
  } else {
    lcdDisplay.printMessage("Ethernet OK", 0, 0);

    float speed = ethernetModule.measureInternetSpeed("www.google.com", "/testfile");
    if (speed > 0) {
      Serial.print("Speed: ");
      Serial.print(speed);
      Serial.println(" KB/s");
      lcdDisplay.printMessage("Speed: ", 0, 1);
      lcdDisplay.printMessage(String(speed).c_str(), 7, 1);
      lcdDisplay.printMessage(" KB/s", 12, 1);
    } else {
      Serial.println("Failed to measure speed");
      lcdDisplay.printMessage("Speed Test Fail", 0, 1);
    }
  }
  delay(1000);
}
