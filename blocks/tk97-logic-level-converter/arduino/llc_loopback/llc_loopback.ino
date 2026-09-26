/*
  Logic Level Converter - loopback check                TK97 / /p/tk97

  Two channels, both ways, with one jumper and no other part. Socket
  at the top, both rows start at GND at the left.

  A 3.3 V board (ESP32, ESP32-S3, Pico) goes on the OUT side:

    GND    -> GND
    5V     -> 5V (VBUS on a Pico). The TK97 makes its own 3.3 V.
    3V3    -> nothing. It is the TK97's output, never your 3V3.
    L1     -> OUT_PIN
    L2     -> BACK_PIN
    H1, H2    joined to each other with one jumper.

  An Uno goes on the IN side: GND and 5V the same, H1 -> OUT_PIN,
  H2 -> BACK_PIN, and one jumper from L1 to L2 in the socket.

  OUT_PIN goes out through channel 1, over the jumper and back
  through channel 2 to BACK_PIN.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
    Serial Monitor                115200
*/

// The pin wired to L1 (H1 on an Uno).
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 14.
const int OUT_PIN = 4;
// The pin wired to L2 (H2 on an Uno).
// Uno: 3. ESP32: 26. ESP32-S3: 5. Pico: 15.
const int BACK_PIN = 5;

// Time for a level to cross twice before it is read. Microseconds
// would do; a millisecond keeps it obvious.
const int SETTLE_MS = 1;

int sendAndRead(int level) {
  digitalWrite(OUT_PIN, level);
  delay(SETTLE_MS);
  return digitalRead(BACK_PIN);
}

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, OUTPUT);
  // No pull-up here: the TK97's own 10 kOhm holds the line.
  pinMode(BACK_PIN, INPUT);
}

void loop() {
  int high = sendAndRead(HIGH);
  int low = sendAndRead(LOW);

  if (high == HIGH && low == LOW) {
    Serial.println("HIGH came back HIGH, LOW came back LOW: ok");
  } else if (high == HIGH && low == HIGH) {
    Serial.println("always HIGH: check the jumper, and BACK_PIN");
  } else if (high == LOW && low == LOW) {
    Serial.println("always LOW: check 5V and GND, then OUT_PIN");
  } else {
    Serial.println("backwards: check every wire against the list");
  }
  delay(1000);
}
