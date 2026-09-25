// EEPROM save LED state: button toggles and writes; on power-up read and restore
#include <Wire.h>
#define EEPROM_ADDR 0x50   // TK31 I2C address
#define BUTTON_PIN 2
#define LED_PIN 13

int lastButtonState = HIGH;
bool ledState = false;

void setup() {
  Wire.begin();   // I2C init (SDA=A4, SCL=A5)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  
  // Read last state from EEPROM
  byte savedState = readEEPROM(0);
  ledState = (savedState == 1);
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  
  Serial.print("Restored: LED ");
  Serial.println(ledState ? "on" : "off");
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  
  // Button press (HIGH -> LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    ledState = !ledState;   // Toggle LED
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    
    // Save to EEPROM
    writeEEPROM(0, ledState ? 1 : 0);
    
    Serial.print("Saved: LED ");
    Serial.println(ledState ? "on" : "off");
    
    delay(200);
  }
  
  lastButtonState = buttonState;   // For edge detection
  delay(50);   // Debounce
}

void writeEEPROM(int address, byte data) {   // I2C write one byte
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(address >> 8);
  Wire.write(address & 0xFF);
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}

byte readEEPROM(int address) {   // I2C read one byte
  byte data = 0;
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(address >> 8);
  Wire.write(address & 0xFF);
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADDR, 1);
  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}
