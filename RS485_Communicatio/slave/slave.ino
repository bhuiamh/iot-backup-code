#define LED       13
#define SLAVE_EN  8

void setup() {
  pinMode(LED , OUTPUT);                        // Declare LED pin as output
  pinMode(SLAVE_EN , OUTPUT);                   // Declare Enable pin as output
  Serial.begin(9600);                           // set serial communication baudrate 
  digitalWrite(SLAVE_EN , LOW);                 // Make Enable pin low
                                                // Receiving mode ON 
}

void loop() {
  while(Serial.available())                     // If serial data is available then enter into while loop
  {
    if(Serial.read() == 'R')                    // if available data is A
    {
      Serial.println("Received");
      digitalWrite(LED , !digitalRead(LED));    // LED Blink
    }
  }
}