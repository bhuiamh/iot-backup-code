
#define BLYNK_PRINT Serial

/*Blynk Device Info*/
#define BLYNK_TEMPLATE_ID "TMPL62dTnsvrD"
#define BLYNK_TEMPLATE_NAME "Bike Guard by Electrical Ghost"
#define BLYNK_AUTH_TOKEN "3axZFQRZJsMfFGpWaLFsrBQrhFQIMlXm"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials.
char ssid[] = "Musa Personal";
char pass[] = "hello2441139";


void setup()
{
  pinMode(16, OUTPUT); // For D0
  pinMode(5, OUTPUT);  // For D1
  pinMode(14, OUTPUT);  // For D5
  pinMode(12, OUTPUT);  // For D6
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

 
}
//  BLYNK_WRITE(V1){
//     int value = param.asInt();\images\tutorial\esp8266-nodemcu-pinout.jpg
//     value ? digitalWrite(LED_BUILTIN, HIGH): digitalWrite(LED_BUILTIN, LOW);
//   }
 BLYNK_WRITE(V0){
    int value = param.asInt();
    value ? digitalWrite(16, HIGH): digitalWrite(16, LOW);
  }
 BLYNK_WRITE(V1){
    int value = param.asInt();
    value ? digitalWrite(5, HIGH): digitalWrite(5, LOW);
  }
 BLYNK_WRITE(V2){
    int value = param.asInt();
    value ? digitalWrite(14, HIGH): digitalWrite(14, LOW);
  }
 BLYNK_WRITE(V3){
    int value = param.asInt();
    value ? digitalWrite(12, HIGH): digitalWrite(12, LOW);
  }
//  BLYNK_WRITE(V4){
//     int value = param.asInt();
//     value ? digitalWrite(12, HIGH): digitalWrite(12, LOW);
//   }


void loop()
{
  Blynk.run();
}

