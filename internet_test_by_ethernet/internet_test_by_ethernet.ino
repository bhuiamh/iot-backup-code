#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xCE, 0xEF, 0xFE, 0xEC };
EthernetClient client;

void setup() {
  Serial.begin(9600);
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true) {
      delay(1);
    }
  }
  Serial.print("  DHCP assigned IP ");
  Serial.println(Ethernet.localIP());
  
  // Attempt to connect to a known IP address
  IPAddress server(93, 184, 216, 34); // example.com IP address
  Serial.println("Attempting to connect to example.com...");

  if (client.connect(server, 80)) {
    Serial.println("Connected to example.com");
    client.stop();
  } else {
    Serial.println("Connection to example.com failed");
  }
}

void loop() {
  // Nothing to do here
}
