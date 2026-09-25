// CLOCK, DATA and BTN, as GPIO numbers.
// Uno: 2, 3, 4. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
const int CLOCK_PIN = 4;
const int DATA_PIN = 5;

long count = 0;
int lastClock = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(CLOCK_PIN, INPUT);    // the block has its own pull-ups
  pinMode(DATA_PIN, INPUT);
}

void loop() {
  int clockLevel = digitalRead(CLOCK_PIN);
  if (clockLevel != lastClock) {
    if (clockLevel == LOW) {    // CLOCK has just fallen
      if (digitalRead(DATA_PIN) == HIGH) count++;   // CLOCK first
      else count--;                                 // DATA first
      Serial.println(count);
    }
    lastClock = clockLevel;
  }
}
