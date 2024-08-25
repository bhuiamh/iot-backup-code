#include <ESP8266WiFi.h>         // For ESP8266 Wi-Fi
#include <PubSubClient.h>        // For MQTT

// Replace these with your network credentials
const char* ssid = "Musa Personal";
const char* password = "hello2441139";

// Replace with your MQTT broker's IP and port
const char* mqttServer = "91.121.93.94"; // Example IP, replace with your broker's IP
const int mqttPort = 1883;

// Create instances for Wi-Fi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

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
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("yourTopic"); // Subscribe to the desired topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup MQTT server and callback
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Example to publish a message
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    String message = "Hello from ESP8266!";
    if (client.publish("yourTopic", message.c_str())) {
      Serial.println("Message sent: " + message);
    } else {
      Serial.println("Message send failed");
    }
  }
}
