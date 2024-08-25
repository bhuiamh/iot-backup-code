#include "EthernetModule.h"

EthernetModule::EthernetModule(byte* mac) : mac(mac) {}

bool EthernetModule::initialize() {
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        return false;
    }
    delay(1000); // Give the Ethernet shield a second to initialize
    return true;
}

float EthernetModule::measureInternetSpeed(const char* host, const char* path) {
    EthernetClient client;
    if (client.connect(host, 80)) {
        client.print("GET ");
        client.print(path);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(host);
        client.println("Connection: close");
        client.println();

        unsigned long startTime = millis();
        int bytesRead = 0;
        const int testDataSize = 10000;

        while (client.connected() && (millis() - startTime) < 10000) { // 10 seconds timeout
            if (client.available()) {
                char c = client.read();
                bytesRead++;
            }
            if (bytesRead >= testDataSize) break; // Stop after reading the test data size
        }

        unsigned long endTime = millis();
        client.stop();

        float duration = (endTime - startTime) / 1000.0; // time in seconds
        float speed = (bytesRead / 1024.0) / duration; // speed in KB/s
        return speed;
    } else {
        client.stop();
        return -1; // Connection failed
    }
}
j