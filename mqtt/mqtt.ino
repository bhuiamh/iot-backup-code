#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[] =  { 0xDE, 0xBD, 0xEE, 0xEF, 0xFE, 0xEC };

EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Confirm the message is received and processed
  client.publish("yourTopic/ack", "Message received");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      client.subscribe("yourTopic"); // Subscribe to the desired topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 10 seconds");
      delay(10000); // Increase the delay to 10 seconds
    }
  }
}

void setup() {
  Serial.begin(9600);

  // Initialize Ethernet with DHCP
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // If DHCP fails, use a static IP:
    IPAddress ip(192, 168, 1, 175); // Change to your network's IP
    IPAddress myDns(192, 168, 1, 1); // Change to your network's DNS server
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  // Manually resolve the IP address of test.mosquitto.org (91.121.93.94)
  IPAddress server(91, 121, 93, 94); // Replace with the actual IP address resolved manually

  client.setServer(server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  // Ensure connection before publishing
  if (client.connected()) {
    client.loop();

    static unsigned long lastMsg = 0;
    unsigned long now = millis();
    if (now - lastMsg > 5000) {  // Publish every 5 seconds instead of 2
      lastMsg = now;
      String message = "Hello from Arduino!";
      client.publish("yourTopic", message.c_str());
    }
  }
}
