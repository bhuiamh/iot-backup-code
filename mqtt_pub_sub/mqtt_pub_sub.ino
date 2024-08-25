#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// MAC address of your Ethernet shield (modify as needed)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// IP address of the MQTT broker (replace with your broker's IP address)
IPAddress server(10, 200, 10, 102); 

EthernetClient ethClient;
PubSubClient client(ethClient);

bool variable = true; // Replace with your actual condition
String eightDigitNumber = "12345678"; // Example 8-digit number
const int ledPin = 7; // Pin where the LED is connected

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  // Convert payload to string
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  // Check if the received message is '1'
  if (message == "1") {
    digitalWrite(ledPin, HIGH); // Turn on LED
    Serial.println("LED turned ON");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      client.subscribe("responseTopic"); // Subscribe to the response topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Ensure LED is off

  // Initialize Ethernet with DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Use a static IP if DHCP fails
    IPAddress ip(192, 168, 1, 175);
    IPAddress myDns(192, 168, 1, 1);
    Ethernet.begin(mac, ip, myDns);
  }

  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());

  client.setServer(server, 139);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if (variable) { // Check if the condition is true
    Serial.println("Publishing 8-digit number...");
    client.publish("testTopic", eightDigitNumber.c_str()); // Publish the 8-digit number
    variable = false; // Prevent publishing again
  }
}
