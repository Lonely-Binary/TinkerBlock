// A voltmeter readout: the voltage on A0, to two decimals, on a digit panel.
//
// Wiring, display to board:
//   Display GND   -> board GND
//   Display VCC   -> board 5V
//   Display CLOCK -> D2
//   Display DATA  -> D3
// And the thing being measured:
//   Potentiometer outer pins -> 5V and GND, wiper -> A0
//   (or just a jumper from A0 to 5V, 3V3 or GND to test it)
//
// Arduino IDE: board "Arduino Uno", and "TM1637" by Avishay Orpaz from the
// Library Manager. Nothing else to set.

#include <TM1637Display.h>

#define CLOCK_PIN 2
#define DATA_PIN  3
#define POINT     0b10000000   // after the first of the three digits written

TM1637Display display(CLOCK_PIN, DATA_PIN);

void setup() {
  display.setBrightness(4);
  display.clear();             // position 0 is never written below
}

void loop() {
  long mv = (long)analogRead(A0) * 5000 / 1023;   // millivolts, against the 5 V rail
  int centivolts = (mv + 5) / 10;                 // 0 to 500, rounded

  // Three digits starting at position 1, leading zeros on, point after the
  // first of them. 0.05 and 4.98 both read correctly this way; four digits
  // with leading zeros off would blank what it does not need and leave the
  // point sitting beside nothing.
  display.showNumberDecEx(centivolts, POINT, true, 3, 1);
  delay(200);
}
