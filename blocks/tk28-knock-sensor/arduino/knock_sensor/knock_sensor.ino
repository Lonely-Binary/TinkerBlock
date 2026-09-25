// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int KNOCK_PIN = 4;
const unsigned long HOLD_OFF_MS = 100;   // longer than the spring rings

int last = LOW;
unsigned long lastKnock = 0;
unsigned long knocks = 0;

void setup() {
  Serial.begin(115200);
  pinMode(KNOCK_PIN, INPUT);   // the block has its own pull-down
}

void loop() {
  int level = digitalRead(KNOCK_PIN);
  unsigned long now = millis();
  if (level == HIGH && last == LOW) {       // a pulse starts
    if (now - lastKnock >= HOLD_OFF_MS) {   // not the same knock
      knocks++;
      lastKnock = now;
      Serial.print("knocks: ");
      Serial.println(knocks);
    }
  }
  last = level;                // no delay(): it would miss knocks
}
