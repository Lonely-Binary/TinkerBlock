/*
  Infrared Receiver - a beam that breaks                TK64 / /p/tk64

  A TK63 Infrared Transmitter sends the beam; this TK64 reads it.
  Stand them face to face, a few centimetres apart to start.

  Wiring, TK64. Count from the square pad on the TinkerBlock board,
  parts up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno.
              Your board's logic voltage: strong infrared takes
              SIGNAL to about VCC - 0.4 V.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 34 on an ESP32,
              A0 on an Uno, GP26 on a Raspberry Pi Pico

  Wiring, TK63: GND to GND, VCC to the same supply pin as the
  TK64's, NC to nothing, and its SIGNAL to GPIO 6 on an ESP32-S3,
  GPIO 22 on an ESP32, D3 on an Uno, GP17 on a Pico.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin the TK64's SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int IR_PIN = 4;

// The pin the TK63's SIGNAL is wired to. HIGH turns its beam on.
// Uno: 3. ESP32: 22. ESP32-S3: 6. Pico: 17.
const int TX_PIN = 6;

const int SETTLE_MS = 2;   // let the sensor settle after a switch

int clearBeam = 0;         // the beam, measured with the path clear
bool wasBroken = false;

int readIr() {
  long sum = 0;
  for (int i = 0; i < 8; i++) {
    sum += analogRead(IR_PIN);
  }
  return sum / 8;
}

// One round: the room alone, then the room and the beam.
int beam() {
  digitalWrite(TX_PIN, LOW);
  delay(SETTLE_MS);
  int off = readIr();              // the room's own infrared
  digitalWrite(TX_PIN, HIGH);
  delay(SETTLE_MS);
  int on = readIr();               // the room and the beam
  digitalWrite(TX_PIN, LOW);
  return on - off;                 // the beam alone
}

void setup() {
  Serial.begin(115200);
  pinMode(TX_PIN, OUTPUT);
  delay(500);

  // Learn the beam with nothing in the way.
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += beam();
  }
  clearBeam = sum / 20;
  Serial.print("clear beam: ");
  Serial.println(clearBeam);
  if (clearBeam < 20) {
    Serial.println("weak beam: move closer, aim, or shade it");
  }
}

void loop() {
  // Under half the clear beam: something is in the way.
  bool broken = beam() < clearBeam / 2;

  if (broken != wasBroken) {
    Serial.println(broken ? "beam broken" : "beam clear");
    wasBroken = broken;
  }
  delay(10);
}
