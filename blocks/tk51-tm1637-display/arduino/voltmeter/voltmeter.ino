/*
  TM1637 4-Digit Display - a voltmeter readout     TK51 / /p/tk51

  The voltage on A0, to two decimals, on a digit panel.

  Wiring. No pad on this board is square, so go by the names
  printed on the back: GND, VCC, clock, data. Seen from the
  display side with the header on the left, GND is the top pin.

    GND   -> GND
    VCC   -> 5V
    CLOCK -> D2
    DATA  -> D3
  And the thing being measured, 0 to 5 V only:
    a potentiometer's outer pins to 5V and GND, its wiper to A0,
    or a jumper from A0 to 5V, 3V3 or GND to test it.

  Arduino IDE
    Tools > Board            Arduino Uno
    Tools > Port             the one that appears when you plug in
    Library: "TM1637" by Avishay Orpaz, from Tools > Manage
    Libraries. It installs TM1637Display.h.
*/

#include <TM1637Display.h>

#define CLOCK_PIN 2
#define DATA_PIN  3
#define POINT     0b10000000   // after the first digit written
#define RAIL_MV   5000         // measure your 5V pin and put it here

TM1637Display display(CLOCK_PIN, DATA_PIN);

void setup() {
  display.setBrightness(4);
  display.clear();             // position 0 is never written below
}

void loop() {
  long mv = (long)analogRead(A0) * RAIL_MV / 1023;  // millivolts
  int centivolts = (mv + 5) / 10;                    // 0 to 500

  // Three digits from position 1, leading zeros on, the point
  // after the first of them: 0.05 and 4.98 both read right.
  display.showNumberDecEx(centivolts, POINT, true, 3, 1);
  delay(200);
}
