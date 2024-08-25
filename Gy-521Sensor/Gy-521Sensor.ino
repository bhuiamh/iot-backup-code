#include <Wire.h>

const int MPU = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize MPU-6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);     // Wake the sensor up
  Wire.endTransmission(true);
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

  // Print the data
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp / 340.00 + 36.53); // Temperature in degrees C
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);

  delay(333);
}
