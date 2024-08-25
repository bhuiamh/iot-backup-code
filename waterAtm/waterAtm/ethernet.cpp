#include "ethernet.h"

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

EthernetClient ethclient;
PubSubClient mqclient(ethclient);

void initiateEthernet()
{
  Ethernet.init(Ethernet_PIN);
  Ethernet.begin(mac);
  delay(1000);
  Serial.print("Initialize Ethernet. DHCP IP: ");
  Serial.println(Ethernet.localIP());

}
void initiateMqtt(IPAddress mqttServer, char* sub_topic)
{
  mqclient.setServer(mqttServer, 1883);
  mqclient.setCallback(callback);
  mqclient.setBufferSize(512);
  mqclient.connect("random");
  mqclient.subscribe(sub_topic);
}


void callback(char* sub_topic, byte* payload, unsigned int length) {

  Serial.println("Callback Recieved :");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    if (i % 80 == 1) {
      Serial.println();
    }
  }
  Serial.println();

  deserializeJson(docs, payload, length);

  //IF card is valid the StatusCode will be 1
  if (docs["StatusCode"] == 1) {
    //Serial.println("success");
    cardIsValid = true;
    calibrateFlowSensor(docs["Calibration"]);
    cardBalance = docs["Water_Balance"];
    cardIsValid = true;
    cardIsAuthorized = true;
  } else {
    resetflags();
    brokerMsg = docs["Message"].as<String>();
    printToLCD(brokerMsg);
    delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
  }
  callbackReceived = true;
  
}

bool checkinternet(char* sub_topic) {
  int retry = 0;

  // Check if Ethernet link is ON
  if (Ethernet.linkStatus() == LinkON) {

    // Check if MQTT client is already connected
    if (mqclient.connected()) {
      Serial.println("MQTT client already connected.");
      return true;
    } else {
      // Try to connect if not already connected
      while (!mqclient.connected() && retry < 3) {
        Serial.print("Attempting MQTT connection...");
        if (mqclient.connect("arduinoClient")) {
          mqclient.subscribe(sub_topic);
          return true;
          //mqclient.publish(topic_dispense, msg2);
          //break;  // Exit loop if connection is successful
        } else {
          Serial.print("failed, rc=");
          Serial.print(mqclient.state());
          Serial.println(" try again in 5 seconds");
          // Wait 5 seconds before retrying
          delay(5000);
          retry++;
        }
      }
      // If still not connected after retries, set status to false
      if (!mqclient.connected()) {
        // printConnectionError();
        // delay(TIME_TO_SHOW_AUTHORIZATION_ERROR);
        return false;
      }
    }
  } else {
    // If Ethernet link is OFF
    Serial.println("Link off");

    return false;
  }
}


void sendMsg(char* topic, char msg[])
{
  mqclient.publish(topic, msg);

}

void mqloop()
{
  mqclient.loop();
}


