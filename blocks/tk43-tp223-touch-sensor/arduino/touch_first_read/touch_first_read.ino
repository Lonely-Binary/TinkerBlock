/*
  Touch Sensor - first read                              TK43 / /p/tk43

  Wiring. Count from the square pad on the TinkerBlock board, pad
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (touched, SIGNAL gives your pin whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.

  Keep your hand off the pad while the board powers up.
*/

// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int TOUCH_PIN = 4;

int last = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);    // the chip drives SIGNAL both ways
}

void loop() {
  int reading = digitalRead(TOUCH_PIN);

  if (reading != last) {        // print only when it changes
    if (reading == HIGH) {
      Serial.println("HIGH  touched");
    } else {
      Serial.println("LOW   released");
    }
    last = reading;
  }
}
