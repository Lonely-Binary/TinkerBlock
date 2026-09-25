/*
  Touch Sensor - a toggle                                TK43 / /p/tk43

  Wiring. Count from the square pad on the TinkerBlock board, pad
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (touched, SIGNAL gives your pin whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  A TK01 XL LED on a second pin shows the state (optional):
    GND    -> GND
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32, GPIO 5 on an
              ESP32-S3, GP14 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO number the touch block's SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int TOUCH_PIN = 4;
// The GPIO number the TK01's SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 14.
const int LED_PIN = 5;

int last = LOW;
bool ledOn = false;             // the toggle's memory

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);    // the chip drives SIGNAL both ways
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int reading = digitalRead(TOUCH_PIN);

  if (reading != last) {
    if (reading == HIGH) {      // a finger just landed: flip
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      Serial.println(ledOn ? "on" : "off");
    }
    last = reading;
  }
}
