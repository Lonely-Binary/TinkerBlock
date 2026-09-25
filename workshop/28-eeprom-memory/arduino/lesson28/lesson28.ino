// Read/write TK31 EEPROM over I2C: write then read, verify data survives power loss
#include <Wire.h>

#define EEPROM_ADDR 0x50   // TK31 I2C address, fixed at 0x50 on this board

void setup() {
  Wire.begin();            // Init I2C (uses A4 SDA, A5 SCL)
  Serial.begin(9600);
  
  writeEEPROM(0, 123);     // Write byte 123 to address 0
  Serial.println("Data written to EEPROM");
  delay(100);              // Short wait; EEPROM needs time to write
}

void loop() {
  byte data = readEEPROM(0);   // Read one byte from address 0
  Serial.print("Read from EEPROM: ");
  Serial.println(data);
  delay(2000);
}

// Write one byte to address: I2C sends "device addr + addr high + addr low + data"
void writeEEPROM(int address, byte data) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(address >> 8);    // 16-bit address: high byte first
  Wire.write(address & 0xFF);  // then low byte
  Wire.write(data);            // then data
  Wire.endTransmission();      // end transfer; device starts write
  delay(5);                    // EEPROM write takes a few ms
}

// Read one byte from address: send address, then request 1 byte
byte readEEPROM(int address) {
  byte data = 0;
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(address >> 8);
  Wire.write(address & 0xFF);
  Wire.endTransmission();       // end address send
  Wire.requestFrom(EEPROM_ADDR, 1);  // request 1 byte from device
  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}
