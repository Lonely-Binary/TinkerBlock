// TK37 Passive Buzzer: a melody, on an ESP32-S3.
//
// Wiring, TK37 header left to right (parts up, pins down):
//   GND    -> ESP32 GND
//   VCC    -> ESP32 3V3   (5V works too, and is louder)
//   NC     -> nothing
//   SIGNAL -> GPIO4
//
// Arduino IDE: Tools > Board > esp32 > ESP32S3 Dev Module,
// Tools > USB CDC On Boot > Enabled, then Tools > Port.
// No library needed. Serial Monitor at 115200.

// The pin SIGNAL is wired to. Uno: 9. ESP32: 4. Pico: 15.
const int BUZZER_PIN = 4;

// Notes in the sixth octave, in Hz (A4 = 440).
const int C6 = 1047, D6 = 1175, E6 = 1319, F6 = 1397, G6 = 1568;

struct Note {
  int hz;       // the pitch
  float beats;  // the length
};

// Ode to Joy, the opening line.
const Note TUNE[] = {
  {E6, 1}, {E6, 1}, {F6, 1}, {G6, 1},
  {G6, 1}, {F6, 1}, {E6, 1}, {D6, 1},
  {C6, 1}, {C6, 1}, {D6, 1}, {E6, 1},
  {E6, 1.5}, {D6, 0.5}, {D6, 2},
};

const int BEAT_MS = 300;  // one beat: change it for the tempo

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (const Note &n : TUNE) {
    int ms = n.beats * BEAT_MS;
    tone(BUZZER_PIN, n.hz, ms * 9 / 10);  // sound for 90 %
    delay(ms);                            // the rest is the gap
  }
  Serial.println("again in 2 s");
  delay(2000);
}
