/*
  Collision Sensor - counting hits (no debounce yet)     TK17 / /p/tk17

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

int lastReading = LOW;          // what the pin said last time round
unsigned long hits = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUMPER_PIN, INPUT);   // the block has its own pull-down
}

void loop() {
  int reading = digitalRead(BUMPER_PIN);

  if (reading != lastReading) { // the pin has changed
    lastReading = reading;
    if (reading == HIGH) {      // ...to HIGH: a hit, or a bounce
      hits++;
      Serial.print("hits: ");
      Serial.println(hits);
    }
  }
}
