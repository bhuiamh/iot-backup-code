#include <Wire.h>

const int MPU = 0x68; // I2C address of the MPU-6050
const int ALARM = 13; // GPIO pin for the LED
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

float baselineX = 0, baselineY = 0, baselineZ = 0;


bool baselineCollected = false;
unsigned long startMillis;


const int BASELINE_DURATION = 5000; // 5 seconds


const int BaseDiffX = 500;
const int BaseDiffY = 500;
const int BaseDiffZ = 500;


int sampleCount = 0;
int ledOnDuration = 2000; // Initial LED on duration (2 seconds)

void setup() {

  Serial.begin(9600);
  Wire.begin();

  // Initialize MPU-6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Wake the sensor up
  Wire.endTransmission(true);

  pinMode(ALARM, OUTPUT);

  // Start collecting baseline data
  startMillis = millis();
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // Request a total of 14 registers

  AcX = Wire.read() << 8 | Wire.read(); // Acceleration X
  AcY = Wire.read() << 8 | Wire.read(); // Acceleration Y
  AcZ = Wire.read() << 8 | Wire.read(); // Acceleration Z
  Tmp = Wire.read() << 8 | Wire.read(); // Temperature
  GyX = Wire.read() << 8 | Wire.read(); // Gyroscope X
  GyY = Wire.read() << 8 | Wire.read(); // Gyroscope Y
  GyZ = Wire.read() << 8 | Wire.read(); // Gyroscope Z

  if (!baselineCollected) {
    // Collect baseline data for 5 seconds
    baselineX += AcX;
    baselineY += AcY;
    baselineZ += AcZ;
    sampleCount++;

    if (millis() - startMillis >= BASELINE_DURATION) {
      baselineX /= sampleCount;
      baselineY /= sampleCount;
      baselineZ /= sampleCount;

      baselineCollected = true;
      
      digitalWrite(ALARM, HIGH);
      delay(500);
      digitalWrite(ALARM, LOW);

      Serial.print("baselineX: ");
      Serial.print(baselineX);
       Serial.print("|| baselineY: ");
      Serial.print(baselineY);
       Serial.print("|| baselineZ: ");
      Serial.println(baselineZ);

    }
  } else {
   
    float diffX = abs(AcX-baselineX);
    float diffY = abs(AcY-baselineY);
    float diffZ = abs(AcZ-baselineZ);


 if ( diffX > BaseDiffX || diffY > BaseDiffY || diffZ > BaseDiffZ ) {
      // Deviation detected, activate LED
      digitalWrite(ALARM, HIGH);
      delay(ledOnDuration);
      digitalWrite(ALARM, LOW);

      // Increase LED on duration for subsequent deviations
      if (ledOnDuration == 2000) {
        Serial.println("Trigered for 2");
        Serial.print("AcX = "); Serial.print(diffX);
        Serial.print(" | AcY = "); Serial.print(diffY);
        Serial.print(" | AcZ = "); Serial.println(diffZ);
        ledOnDuration = 4000;
      } else if (ledOnDuration == 4000) {
        Serial.println("Trigered for 4");
        Serial.print("AcX = "); Serial.print(diffX);
        Serial.print(" | AcY = "); Serial.print(diffY);
        Serial.print(" | AcZ = "); Serial.println(diffZ);
        ledOnDuration = 6000;
      } else {
        Serial.println("Trigered for 7");
        Serial.print("AcX = "); Serial.print(diffX);
        Serial.print(" | AcY = "); Serial.print(diffY);
        Serial.print(" | AcZ = "); Serial.println(diffZ);
        ledOnDuration = 7000;
      }
    }



  }

  delay(500); // Adjust delay as needed for responsiveness
}
