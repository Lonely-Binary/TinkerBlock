/*
  Push Button - a toggle                                 TK04 / /p/tk04

  Wiring. Count from the square pad on the TinkerBlock board, button
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (pressed, SIGNAL gives your pin whatever VCC is)
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

// The GPIO number the button's SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int BUTTON_PIN = 4;
// The GPIO number the TK01's SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 14.
const int LED_PIN = 5;
const unsigned long DEBOUNCE_MS = 20;

int lastReading = LOW;
int state = LOW;
unsigned long lastChange = 0;
bool ledOn = false;             // the toggle's memory

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);   // the block has its own pull-down
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  unsigned long now = millis();

  if (reading != lastReading) { // the contacts moved: restart the clock
    lastReading = reading;
    lastChange = now;
  }

  if (now - lastChange >= DEBOUNCE_MS && reading != state) {
    state = reading;
    if (state == HIGH) {        // one real press: flip
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      Serial.println(ledOn ? "on" : "off");
    }
  }
}
