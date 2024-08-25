#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xAD, 0xCE, 0xEF, 0xFE, 0xEC };

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
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("test/topic", "hello world");
      // ... and resubscribe
      client.subscribe("test/topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);

  // Initialize Ethernet with DHCP
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true);
  }
  Serial.print("  DHCP assigned IP ");
  Serial.println(Ethernet.localIP());

  // Manually resolve the IP address of test.mosquitto.org
  IPAddress server(91, 121, 93, 94); // Replace with the actual IP address resolved manually

  client.setServer(server, 1883);
  client.setCallback(callback);
  client.setKeepAlive(60); // Set keep-alive interval to 60 seconds

  // Attempt to connect
  reconnect();
}

void loop() {
  // Ensure the client remains connected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
