/*
  Collision Sensor - stop and back off                   TK17 / /p/tk17

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
const unsigned long DEBOUNCE_MS = 20;
const unsigned long BACK_MS = 400;   // reverse for this long
const unsigned long TURN_MS = 300;   // then turn for this long

enum Mode { DRIVE, BACK, TURN };
Mode mode = DRIVE;
unsigned long modeSince = 0;    // millis() when this mode began

int lastReading = LOW;
int bumper = LOW;               // the debounced reading
unsigned long lastChange = 0;

// Your motor driver's calls go in these three. Here they only say
// what they would do, so the sketch runs with just the bumper wired.
void forward()  { Serial.println("forward"); }
void reverse()  { Serial.println("reverse"); }
void turnLeft() { Serial.println("turn left"); }

void enter(Mode next) {
  mode = next;
  modeSince = millis();
  if (next == DRIVE) forward();
  if (next == BACK) reverse();
  if (next == TURN) turnLeft();
}

// True once per hit: the debounced reading has just become HIGH.
bool hit() {
  int reading = digitalRead(BUMPER_PIN);
  unsigned long now = millis();
  if (reading != lastReading) {
    lastReading = reading;
    lastChange = now;
  }
  if (now - lastChange >= DEBOUNCE_MS && reading != bumper) {
    bumper = reading;
    return bumper == HIGH;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUMPER_PIN, INPUT);   // the block has its own pull-down
  enter(DRIVE);
}

void loop() {
  unsigned long inMode = millis() - modeSince;

  if (hit()) {                  // in any mode: back off, now
    enter(BACK);
  } else if (mode == BACK && inMode >= BACK_MS) {
    enter(TURN);
  } else if (mode == TURN && inMode >= TURN_MS) {
    enter(DRIVE);
  }
}
