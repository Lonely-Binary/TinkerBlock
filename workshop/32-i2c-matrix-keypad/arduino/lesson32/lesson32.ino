// I2C scan 4×4 matrix keypad: PCF8574 selects column, read rows for key index, then lookup char
#include <Wire.h>

#define KEYPAD_ADDRESS 0x20   // PCF8574 I2C address

char keymap[16] = "DCBA#9630852*741";   // Key index 0-15 → character

void setup() {
  Wire.begin();   // I2C init
  Serial.begin(9600);
  Serial.println("I2C matrix keypad started");
}

void loop() {
  int key = readKeypad();   // Scan; returns 0-15 or -1
  if (key >= 0 && key < 16) {
    Serial.print("Key: ");
    Serial.println(keymap[key]);
  }
  delay(100);   // Scan interval and debounce
}

// Drive each column low, read rows; low row = key pressed, return 0-15, else -1
int readKeypad() {
  for (int col = 0; col < 4; col++) {
    Wire.beginTransmission(KEYPAD_ADDRESS);
    Wire.write(0xFF & ~(1 << (4 + col)));   // Select column col
    Wire.endTransmission();
    delayMicroseconds(200);
    Wire.requestFrom(KEYPAD_ADDRESS, 1);
    if (Wire.available()) {
      int rowData = Wire.read();
      for (int row = 0; row < 4; row++) {
        if (!(rowData & (1 << row)))
          return row * 4 + col;
      }
    }
  }
  return -1;
}
