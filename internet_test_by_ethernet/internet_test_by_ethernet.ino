#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal_I2C.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Define your static IP address, gateway, and subnet mask
IPAddress ip(192, 168, 1, 177); // Choose an available IP in your network range
IPAddress gateway(192, 168, 1, 1); // Your network's gateway IP
IPAddress subnet(255, 255, 255, 0); // Your network's subnet mask

// Define UDP settings
EthernetUDP udp;
unsigned int localPort = 8888; // Local port to listen for incoming packets
IPAddress remoteIp(192, 168, 1, 100); // Remote IP address to send UDP packets to
unsigned int remotePort = 8888; // Remote port to send UDP packets to

// Initialize the LCD with the I2C address and dimensions
LiquidCrystal_I2C lcd(0x27, 20, 4); // Address 0x27, 20 columns, 4 rows

void setup() {
  Serial.begin(9600);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  Serial.println("Initializing Ethernet with static IP...");

  // Initialize Ethernet with a static IP address
  Ethernet.begin(mac, ip, gateway, subnet);

  // Print the assigned IP address
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(Ethernet.localIP());

  Serial.print("  IP Address: ");
  Serial.println(Ethernet.localIP());

  // Check if Ethernet hardware is present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    lcd.setCursor(0, 1);
    lcd.print("No Shield");
    Serial.println("Ethernet shield was not found.");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Shield Detected");

    Serial.println("Ethernet shield detected.");

    // Check Ethernet link status
    if (Ethernet.linkStatus() == LinkOFF) {
      lcd.setCursor(0, 2);
      lcd.print("Cable: Disconnected");
      Serial.println("Ethernet cable is not connected.");
    } else {
      lcd.setCursor(0, 2);
      lcd.print("Cable: Connected");

      Serial.println("Ethernet cable is connected.");

      // Start the UDP listener on the specified port
      udp.begin(localPort);
      lcd.setCursor(0, 3);
      lcd.print("Listening on ");
      lcd.print(localPort);
      Serial.print("Listening on port ");
      Serial.println(localPort);

      // Send a UDP packet
      sendUdpMessage("Hello, UDP!");
    }
  }
}

void loop() {
  // Handle incoming UDP packets
  int packetSize = udp.parsePacket();
  if (packetSize) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received Packet");
    
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    
    // Read the packet into a buffer
    char buffer[255];
    int len = udp.read(buffer, 255);
    if (len > 0) {
      buffer[len] = 0; // Null-terminate the string
    }
    
    lcd.setCursor(0, 1);
    lcd.print("Size: ");
    lcd.print(packetSize);
    
    lcd.setCursor(0, 2);
    lcd.print("Data: ");
    lcd.print(buffer);
    
    Serial.print("Contents: ");
    Serial.println(buffer);
  }

  delay(1000); // Delay between checks
}

void sendUdpMessage(const char* message) {
  Serial.print("Sending message: ");
  Serial.println(message);
  
  udp.beginPacket(remoteIp, remotePort);
  udp.write(message);
  udp.endPacket();
}
