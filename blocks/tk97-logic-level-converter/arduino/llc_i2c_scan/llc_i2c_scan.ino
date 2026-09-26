/*
  Logic Level Converter - I2C scan                      TK97 / /p/tk97

  A 5 V Uno and a 3.3 V I2C block, such as the TK42, through the
  TK97. Socket at the top, both rows start at GND at the left.

    GND    -> GND
    5V     -> 5V. The TK97 makes the block's 3.3 V from it.
    H1     -> SDA: A4 on an Uno
    H2     -> SCL: A5 on an Uno
    3V3       nothing to wire: the block in the socket takes it.

  The block plugs into the socket's first four places, its GND on
  GND: GND, VCC, SDA, SCL meet GND, 3V3, L1, L2. The TK97's 10 kOhm
  pull-ups are the bus's pull-ups on both sides.

  A 3.3 V board (ESP32 GPIO 21/22, ESP32-S3 GPIO 8/9, Pico GP4/GP5)
  goes on the OUT side instead: SDA to L1, SCL to L2, GND to GND,
  its 5V (VBUS on a Pico) to 5V and its 3V3 to nothing, with a 5 V
  I2C part on H1 and H2.

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed: Wire comes with every board.
    Serial Monitor                115200
*/

#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  // Each board's default SDA and SCL, so nothing names them here.
  Wire.begin();
#if defined(ARDUINO_ARCH_AVR)
  // A bus held LOW (5V or GND missing) must not hang the Uno.
  Wire.setWireTimeout(25000, true);
#endif
}

void loop() {
  int found = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("found 0x");
      if (addr < 16) Serial.print('0');
      Serial.println(addr, HEX);
      found++;
    }
  }
  if (found == 0) {
    Serial.println("nothing answered: 5V, GND, then SDA and SCL");
  } else {
    Serial.print(found);
    Serial.println(" device(s). A TK42 answers at 0x48.");
  }
  delay(3000);
}
