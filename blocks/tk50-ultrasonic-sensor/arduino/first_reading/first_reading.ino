// The first reading: distance over four wires, in GPIO mode.
//
// Wiring, sensor to board. Count the sensor's pins from the SQUARE PAD,
// which is GND. The names are printed on the back only (VCC is printed
// 3-5V), and from the front the order reads backwards:
//
//   TK50 GND  -> board GND
//   TK50 VCC  -> board 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno
//   TK50 ECHO -> Uno D10, ESP32 GPIO 19, ESP32-S3 GPIO 5, Pico GP3
//   TK50 TRIG -> Uno D9,  ESP32 GPIO 18, ESP32-S3 GPIO 4, Pico GP2
//
// Use the 3V3 rail on a 3.3 V board. There is no regulator and no level
// shifter on the TK50, so ECHO answers at whatever you fed VCC -- from
// 3V3 it is a 3.3 V signal and needs no divider.
//
// Arduino IDE: no library and no special Tools settings. Select your board
// and port, set the Serial Monitor to 115200, and upload.
//
// Both jumpers left open, which is how the board arrives. That is GPIO mode.

const int TRIG_PIN = 9;    // Uno D9.   ESP32: 18.  ESP32-S3: 4.  Pico: 2.
const int ECHO_PIN = 10;   // Uno D10.  ESP32: 19.  ESP32-S3: 5.  Pico: 3.

// 25 ms of round trip is about 4 m, which is past anything this sensor can
// honestly see. Without a timeout pulseIn waits a full second instead.
const unsigned long ECHO_TIMEOUT_US = 25000UL;

// cm per microsecond at 20 C. Warmer air is faster -- see "Warm air is
// fast air" for when that stops being a rounding error.
const float CM_PER_US = 0.0343;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  // Start a measurement. The supported library holds TRIG high for 500 us,
  // so this does too. HC-SR04 examples send 10 us.
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(500);
  digitalWrite(TRIG_PIN, LOW);

  // ECHO stays high for the round trip. Zero means the timeout expired.
  unsigned long us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (us == 0) {
    // Not a distance. Absorbed, angled away, out of range, or nothing
    // there at all -- and there is no way to tell which from here.
    Serial.println("no echo");
  } else {
    // Halve it: the sound crossed the gap twice.
    float cm = (us / 2.0) * CM_PER_US;
    Serial.print(cm, 1);
    Serial.println(" cm");
  }

  delay(100);   // One measurement cycle is 50 ms. Let the last burst die.
}
