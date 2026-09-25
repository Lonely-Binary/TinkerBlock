// A distance over a bus: one command byte out, three bytes back.
//
// Wiring, sensor to board. Count the sensor's pins from the SQUARE PAD,
// which is GND:
//
//   TK50 GND  -> board GND
//   TK50 VCC  -> board 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno
//   TK50 ECHO -> I2C: board SDA.  UART: board RX.
//   TK50 TRIG -> I2C: board SCL.  UART: board TX.
//
// UART names are the sensor's own, so they cross: the sensor's TX (on the
// ECHO pin) goes to your board's RX.
//
// SOLDER FIRST. This sketch needs a bridged jumper and will read nothing
// on an out-of-the-box board:
//   I2C  -> bridge J1, leave J2 open
//   UART -> bridge J2, leave J1 open
// J2 is the upper pad. Power the board off to solder and on again after.
//
// Arduino IDE: no library beyond the bundled Wire. No special Tools
// settings. Serial Monitor at 115200.

#include <Wire.h>

#define USE_I2C 1        // 1 for I2C (J1 bridged), 0 for UART (J2 bridged)

const uint8_t  TK50_ADDR    = 0x57;   // Fixed. There is no address pad.
const uint8_t  CMD_I2C      = 0x01;   // "take a reading"
const uint8_t  CMD_UART     = 0xA0;   // the same command, over serial
const uint32_t UART_BAUD    = 9600;   // fixed by the sensor
const uint16_t SETTLE_MS    = 150;    // the burst, the flight, the echo

// Three bytes, most significant first, in MICROMETRES. 10 000 um = 1 cm.
const float UM_PER_CM = 10000.0;

void setup() {
  Serial.begin(115200);
#if USE_I2C
  Wire.begin();
#else
  Serial1.begin(UART_BAUD);   // Uno: use SoftwareSerial instead
#endif
}

// Returns centimetres, or -1 when the sensor did not answer.
float readDistanceCm() {
  uint8_t b[3];

#if USE_I2C
  Wire.beginTransmission(TK50_ADDR);
  Wire.write(CMD_I2C);
  if (Wire.endTransmission() != 0) return -1;   // nobody at that address

  delay(SETTLE_MS);

  // Ask for exactly three, and insist on getting three. Reading fewer and
  // using them anyway is how a dropped byte becomes a plausible distance.
  if (Wire.requestFrom(TK50_ADDR, (uint8_t)3) != 3) return -1;
  for (uint8_t i = 0; i < 3; i++) b[i] = Wire.read();
#else
  while (Serial1.available()) Serial1.read();   // clear anything stale
  Serial1.write(CMD_UART);

  delay(SETTLE_MS);

  if (Serial1.available() < 3) return -1;       // short reply, not a reading
  for (uint8_t i = 0; i < 3; i++) b[i] = Serial1.read();
#endif

  uint32_t um = ((uint32_t)b[0] << 16) | ((uint32_t)b[1] << 8) | b[2];
  return um / UM_PER_CM;
}

void loop() {
  float cm = readDistanceCm();

  if (cm < 0) {
    Serial.println("no answer");
  } else {
    Serial.print(cm, 2);
    Serial.println(" cm");
  }

  delay(100);   // One bus measurement cycle is 100 ms.
}
