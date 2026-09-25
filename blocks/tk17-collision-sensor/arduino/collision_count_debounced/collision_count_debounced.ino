/*
  Collision Sensor - counting hits, debounced            TK17 / /p/tk17

  Wiring. Count from the square pad on the TinkerBlock board, switch
  at the top, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (on a hit, SIGNAL gives your pin whatever VCC is)
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
const int BUMPER_PIN = 4;
const unsigned long DEBOUNCE_MS = 20;   // longer than bounce lasts

int lastReading = LOW;          // what the pin said last time round
int state = LOW;                // what we have decided the bumper is
unsigned long lastChange = 0;   // millis() when the pin last moved
unsigned long hits = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUMPER_PIN, INPUT);   // the block has its own pull-down
}

void loop() {
  int reading = digitalRead(BUMPER_PIN);
  unsigned long now = millis();

  if (reading != lastReading) { // the contacts moved: restart the clock
    lastReading = reading;
    lastChange = now;
  }

  // Held still for DEBOUNCE_MS, and different from what we believed?
  if (now - lastChange >= DEBOUNCE_MS && reading != state) {
    state = reading;
    if (state == HIGH) {        // one real hit
      hits++;
      Serial.print("hits: ");
      Serial.println(hits);
    }
  }

  // Steering, motors, other sensors go here. None of it waits.
}
