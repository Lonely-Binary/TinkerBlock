#include <LedControl.h>

// Pin number: change these to match your wiring
// Demo program uses: DIN=3, CLK=4, CS=2
// If using hardware SPI: DIN=11(MOSI), CLK=13(SCK), CS=10
// If using software SPI: can use any digital pins
#define CS_PIN 2    // Arduino digital pin connected to CS (demo uses D2)
#define DIN_PIN 3   // Arduino digital pin connected to DIN (demo uses D3)
#define CLK_PIN 4   // Arduino digital pin connected to CLK (demo uses D4)

// 8×8 matrix LED configuration (using MAX7219 driver, LedControl library supports software SPI)
// Note: the last argument is the number of 8×8 modules on the chain, not the number of boards
// LedControl constructor: LedControl(dataPin, clockPin, csPin, numDevices)
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);  // Single 8×8 module

// Pattern data (8×8 dot matrix, each pattern 8 bytes)
// Heart pattern
const byte heartPattern[8] = {
  0b00000000,
  0b01100110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000
};

// Triangle pattern
const byte trianglePattern[8] = {
  0b00000000,
  0b00010000,
  0b00111000,
  0b01111100,
  0b11111110,
  0b01111100,
  0b00111000,
  0b00010000
};

// Square pattern
const byte squarePattern[8] = {
  0b11111111,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b11111111
};

// Circle pattern
const byte circlePattern[8] = {
  0b00111100,
  0b01111110,
  0b11000011,
  0b10000001,
  0b10000001,
  0b11000011,
  0b01111110,
  0b00111100
};

// Star pattern
const byte starPattern[8] = {
  0b00011000,
  0b00111100,
  0b01111110,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00000000
};

// Arrow pattern
const byte arrowPattern[8] = {
  0b00001000,
  0b00011100,
  0b00111110,
  0b01111111,
  0b00011100,
  0b00011100,
  0b00011100,
  0b00000000
};

// Smile pattern
const byte smilePattern[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

// Pattern array for easy looping
const byte* patterns[] = {
  heartPattern,      // 0: Heart
  trianglePattern,   // 1: Triangle
  squarePattern,     // 2: Square
  circlePattern,     // 3: Circle
  starPattern,       // 4: Star
  arrowPattern,      // 5: Arrow
  smilePattern       // 6: Smile
};

const char* patternNames[] = {
  "Heart",
  "Triangle",
  "Square",
  "Circle",
  "Star",
  "Arrow",
  "Smile"
};

const int patternCount = 7;  // Number of patterns

// Reverse byte bit order (fix mirror display issue)
byte reverseByte(byte b) {
  byte result = 0;
  for (int i = 0; i < 8; i++) {
    result <<= 1;
    result |= (b & 1);
    b >>= 1;
  }
  return result;
}

// Display pattern
void displayPattern(const byte* pattern) {
  // Clear display
  lc.clearDisplay(0);
  
  // Display pattern (reference demo program implementation)
  // Reverse byte bit order to fix mirror display issue
  for (int i = 0; i < 8; i++) {
    byte reversedByte = reverseByte(pattern[i]);
    lc.setRow(0, i, reversedByte);
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  delay(100);
  
  Serial.println("8×8 matrix LED program started");
  Serial.println("Cycling through various patterns: Heart, Triangle, Square, Circle, Star, Arrow, Smile");
  
  // Initialize matrix LED (LedControl library)
  lc.shutdown(0, false);  // Wake up module 0
  lc.setIntensity(0, 8);  // Set brightness (0-15)
  lc.clearDisplay(0);     // Clear display
  delay(500);
}

void loop() {
  // Cycle through all patterns
  for (int i = 0; i < patternCount; i++) {
    displayPattern(patterns[i]);
    Serial.print("Display pattern: ");
    Serial.println(patternNames[i]);
    delay(1000);  // Switch pattern every second
  }
}
