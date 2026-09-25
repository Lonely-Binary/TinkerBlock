/*
  Push Button - debounced with millis()                  TK04 / /p/tk04

  Wiring. Count from the square pad on the TinkerBlock board, button
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (pressed, SIGNAL gives your pin whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int BUTTON_PIN = 4;
const unsigned long DEBOUNCE_MS = 20;   // longer than bounce lasts

int lastReading = LOW;          // what the pin said last time round
int state = LOW;                // what we have decided the button is
unsigned long lastChange = 0;   // millis() when the pin last moved
unsigned long presses = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);   // the block has its own pull-down
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  unsigned long now = millis();

  if (reading != lastReading) { // the contacts moved: restart the clock
    lastReading = reading;
    lastChange = now;
  }

  // Held still for DEBOUNCE_MS, and different from what we believed?
  if (now - lastChange >= DEBOUNCE_MS && reading != state) {
    state = reading;
    if (state == HIGH) {        // one real press
      presses++;
      Serial.print("presses: ");
      Serial.println(presses);
    }
  }

  // Anything else goes here. None of it waits for the button.
}
