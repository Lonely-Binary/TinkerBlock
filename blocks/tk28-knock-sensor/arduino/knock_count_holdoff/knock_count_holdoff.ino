/*
  Knock Sensor - counting knocks, with a hold-off        TK28 / /p/tk28

  Wiring. Count from the square pad on the TinkerBlock board, switch
  at the top, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (during a knock, SIGNAL gives your pin whatever VCC is)
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
const int KNOCK_PIN = 4;
const unsigned long HOLD_OFF_MS = 100;  // longer than the spring rings

int last = LOW;                 // the reading last time round
unsigned long lastKnock = 0;    // millis() of the last counted knock
unsigned long knocks = 0;

void setup() {
  Serial.begin(115200);
  pinMode(KNOCK_PIN, INPUT);    // the block has its own pull-down
}

void loop() {
  int level = digitalRead(KNOCK_PIN);
  unsigned long now = millis();

  if (level == HIGH && last == LOW) {   // a pulse starts
    if (now - lastKnock >= HOLD_OFF_MS) {   // not the same knock
      knocks++;
      lastKnock = now;
      Serial.print("knocks: ");
      Serial.println(knocks);
    }
  }
  last = level;

  // Other work goes here. None of it may wait.
}
