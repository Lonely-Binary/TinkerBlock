// Read LM75 temperature register over I2C; combine two bytes and convert to °C
#include <Wire.h>

#define LM75_ADDRESS 0x48   // LM75 I2C address (7-bit)

void setup() {
  Wire.begin();   // I2C init (SDA=A4, SCL=A5)
  Serial.begin(9600);
  Serial.println("I2C temperature sensor started");
}

void loop() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00);   // Temperature register address
  Wire.endTransmission();
  Wire.requestFrom(LM75_ADDRESS, 2);   // Request 2 bytes
  if (Wire.available() >= 2) {
    int highByte = Wire.read();
    int lowByte = Wire.read();
    int tempRaw = (highByte << 8) | lowByte;   // 16-bit raw
    float temperature = (tempRaw >> 5) * 0.125;  // LM75: upper 11 bits, 0.125°C per step
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
  }
  delay(1000);   // Update every second
}
