/*
  Infrared Transmitter - a beam you can break      TK63 + TK64 / /p/tk63

  Wiring. Count from the square pad on each TinkerBlock board, parts
  up, header at the bottom:

    TK63 transmitter
      GND    -> GND
      VCC    -> 5V, or VBUS on a Pico (only feeds the LEDs)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> D3, GPIO 22, GPIO 6 or GP17

    TK64 receiver
      GND    -> GND
      VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> A0, GPIO 34, GPIO 4 or GP26

  Uno, ESP32, ESP32-S3, Pico, in that order. Stand the two blocks
  face to face, the TK63's LED pointing at the TK64's, and keep the
  beam clear while the sketch starts: it measures the clear beam.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
    Serial Monitor                115200
*/

// The pin the TK63's SIGNAL is wired to.
// Uno: 3. ESP32: 22. ESP32-S3: 6. Pico: 17.
const int IR_PIN = 6;
// The pin the TK64's SIGNAL is wired to: an analog input.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SENSE_PIN = 4;

int clearBeam = 0;     // the TK63's own light, beam clear
bool broken = false;

// Read the TK64 with the TK63 off, then on. The room's infrared is
// in both readings; the difference is the TK63's light alone.
int beam() {
  digitalWrite(IR_PIN, LOW);
  delay(2);
  int dark = analogRead(SENSE_PIN);
  digitalWrite(IR_PIN, HIGH);
  delay(2);
  int lit = analogRead(SENSE_PIN);
  digitalWrite(IR_PIN, LOW);
  return lit - dark;
}

void setup() {
  pinMode(IR_PIN, OUTPUT);
  digitalWrite(IR_PIN, LOW);
  Serial.begin(115200);
  delay(500);

  long sum = 0;
  for (int i = 0; i < 16; i++) sum += beam();
  clearBeam = sum / 16;
  Serial.print("clear beam: ");
  Serial.println(clearBeam);
}

void loop() {
  // Broken when less than half the clear beam gets through.
  bool now = beam() < clearBeam / 2;
  if (now != broken) {
    broken = now;
    Serial.println(broken ? "beam broken" : "beam clear");
  }
  delay(20);
}
