#include <LedControl.h>

#define DIN_PIN 3   // DIN on D3
#define CLK_PIN 4   // CLK on D4
#define CS_PIN 2    // CS on D2

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

void setup() {
  lc.shutdown(0, false);   // Wake matrix
  lc.setIntensity(0, 8);   // Brightness 0-15
  lc.clearDisplay(0);      // Clear screen
}

void loop() {
  // Heart pattern
  byte heart[8] = {
    0b00000000,
    0b01100110,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00000000
  };
  
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, heart[i]);   // Write row i (0 = first block)
  }
  delay(1000);   // Show 1 s
  lc.clearDisplay(0);   // Clear matrix
  delay(500);   // 0.5 s before repeat
}
