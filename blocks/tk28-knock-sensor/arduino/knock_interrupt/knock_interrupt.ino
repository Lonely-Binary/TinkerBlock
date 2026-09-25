/*
  Knock Sensor - counted by an interrupt                 TK28 / /p/tk28

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

#ifndef IRAM_ATTR
#define IRAM_ATTR               // only the ESP32 cores need it
#endif

volatile unsigned long knocks = 0;      // changed inside onKnock
volatile unsigned long lastKnock = 0;

// Runs the instant SIGNAL goes HIGH. Short, and no printing.
void IRAM_ATTR onKnock() {
  unsigned long now = millis();
  if (now - lastKnock >= HOLD_OFF_MS) {  // not the same knock
    knocks++;
    lastKnock = now;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(KNOCK_PIN, INPUT);    // the block has its own pull-down
  attachInterrupt(digitalPinToInterrupt(KNOCK_PIN), onKnock, RISING);
}

void loop() {
  static unsigned long shown = 0;

  noInterrupts();               // copy it in one piece
  unsigned long n = knocks;
  interrupts();

  if (n != shown) {
    shown = n;
    Serial.print("knocks: ");
    Serial.println(n);
  }
  delay(100);                   // busy elsewhere: the count survives
}
