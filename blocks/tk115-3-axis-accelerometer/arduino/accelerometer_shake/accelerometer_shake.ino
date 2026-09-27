/*
  3-Axis Accelerometer - shake counter                  TK115 / /p/tk115

  Wiring. Count from the square pad, which is GND. Chip side up,
  header at the bottom, left to right:

    GND -> GND
    3V3 -> 3V3      (3.3 V only. The chip's limit is 3.6 V, and the
                     board's pull-ups put this pin on SDA and SCL.)
    SCL -> GPIO 9 on an ESP32-S3, GPIO 22 on an ESP32, GP5 on a Pico
    SDA -> GPIO 8 on an ESP32-S3, GPIO 21 on an ESP32, GP4 on a Pico

  Each board's default I2C pins, so nothing in the sketch names them.
  A 5 V Arduino Uno needs a level converter (TK97) in between.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Library Manager               nothing to install, only Wire
    Serial Monitor                115200
*/

#include <Wire.h>

// 0x19, because the board leaves the chip's SDO pin open and the chip
// pulls it high itself. Tied to GND it would answer at 0x18.
const uint8_t ACCEL_ADDR = 0x19;

bool writeReg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(ACCEL_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

// Read n registers from reg on. Bit 7 set on the register number makes
// the chip step to the next register after every byte.
bool readRegs(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(ACCEL_ADDR);
  Wire.write(reg | 0x80);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(ACCEL_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// X, Y and Z in g. Low byte first; the 12 bits sit at the top of the
// 16, so shift them down, and at +-2 g every count is then 1 mg.
bool readG(float &x, float &y, float &z) {
  uint8_t b[6];
  if (!readRegs(0x28, b, 6)) return false;       // OUT_X_L .. OUT_Z_H
  x = ((int16_t)(b[1] << 8 | b[0]) >> 4) / 1000.0;
  y = ((int16_t)(b[3] << 8 | b[2]) >> 4) / 1000.0;
  z = ((int16_t)(b[5] << 8 | b[4]) >> 4) / 1000.0;
  return true;
}

const float JOLT_G = 0.5;           // this far from 1 g is a jolt
const unsigned long QUIET_MS = 300; // and one jolt is counted once

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);        // native USB: wait for the monitor
  Wire.begin();

  uint8_t id = 0;                    // WHO_AM_I: 0x11 on this chip
  if (!readRegs(0x0F, &id, 1) || id != 0x11) {
    Serial.println("no SC7A20 at 0x19: check GND, then SDA and SCL");
    while (true) delay(100);
  }

  // It powers up asleep. CTRL_REG1: 400 readings a second, X Y Z on.
  writeReg(0x20, 0x77);
  // CTRL_REG4: +-2 g, and never half of one reading and half the next.
  writeReg(0x23, 0x80);
}

unsigned long lastJolt = 0;
unsigned int jolts = 0;

void loop() {
  float x, y, z;
  if (!readG(x, y, z)) return;

  // Still, the total is 1 g whichever way up the board is. A knock or
  // a shake is anything that pushes it away from 1 g.
  float total = sqrt(x * x + y * y + z * z);
  bool jolt = fabs(total - 1.0) > JOLT_G;
  if (jolt && millis() - lastJolt > QUIET_MS) {
    lastJolt = millis();
    jolts++;
    Serial.print("jolt ");  Serial.print(jolts);
    Serial.print("  ");     Serial.print(total, 2);
    Serial.println(" g");
  }
  delay(2);                          // a new reading every 2.5 ms
}
