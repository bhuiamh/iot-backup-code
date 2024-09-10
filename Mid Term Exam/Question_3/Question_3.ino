#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Data wire is connected to Arduino pin 7
#define ONE_WIRE_BUS 7

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

byte mac[] = { 0xDE, 0xAD, 0xCE, 0xEF, 0xFE, 0xEC };

// Ethernet and MQTT clients
EthernetClient ethClient;
PubSubClient client(ethClient);

// Replace with your MQTT server's IP
IPAddress server(192, 168, 1, 102); 

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Start the Ethernet connection with DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // No point in carrying on, so you reset or halt
    while (true);
  }
  
  // Print the assigned IP address
  Serial.print("Assigned IP: ");
  Serial.println(Ethernet.localIP());

  client.setServer(server, 1883);

  // Start up the DS18B20 library
  sensors.begin();
}

void loop() {
  // Ensure the client is connected to the MQTT broker
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Request temperature from DS18B20
  sensors.requestTemperatures(); 
  float temperature = sensors.getTempCByIndex(0);

  // Print temperature to Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Convert temperature to string and publish to MQTT topic
  char tempString[8];
  dtostrf(temperature, 6, 2, tempString);
  client.publish("temp_data", tempString);

  // Wait 5 seconds before next reading
  delay(5000);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
