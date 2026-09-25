/*
  Vibration Motor - patterns on a button                  TK30 / /p/tk30

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: an Uno's 5V, the 5V pin of an ESP32 or ESP32-S3
              board on USB, a Pico's VBUS (3V3 works as well)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32 or ESP32-S3,
              GP15 on a Raspberry Pi Pico

  TK04 Push Button: GND to GND, VCC to your board's logic supply
  (5V on an Uno, 3V3 on the others), SIGNAL to D2 on an Uno, GPIO 25
  on an ESP32, GPIO 5 on an ESP32-S3, GP14 on a Pico.

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 4. Pico: 15.
const int MOTOR_PIN = 9;
// The TK04's SIGNAL. Uno: 2. ESP32: 25. ESP32-S3: 5. Pico: 14.
const int BUTTON_PIN = 2;

// On, off, on ... in ms, on first. 0 ends the list.
const unsigned int TAP_MS[] = {100, 0};
const unsigned int DOUBLE_MS[] = {100, 120, 100, 0};
const unsigned int LONG_MS[] = {400, 0};
const unsigned int ALARM_MS[] = {200, 200, 200, 200, 200, 0};

const int READ_MS = 20;             // read the button this often

const unsigned int *pattern = nullptr;   // playing, or none
int at = 0;                              // which time in it
unsigned long since = 0;                 // when that time began
unsigned long lastRead = 0;
bool wasPressed = false;

void play(const unsigned int *p) {
  pattern = p;
  at = 0;
  since = millis();
  digitalWrite(MOTOR_PIN, HIGH);         // every pattern starts on
}

void update() {
  if (pattern == nullptr) return;
  if (millis() - since < pattern[at]) return;
  at++;
  since = millis();
  if (pattern[at] == 0) {                // the end of the list
    pattern = nullptr;
    digitalWrite(MOTOR_PIN, LOW);
    return;
  }
  digitalWrite(MOTOR_PIN, at % 2 == 0 ? HIGH : LOW);   // even: on
}

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);   // the TK04 has its own pull-down
}

void loop() {
  if (millis() - lastRead >= READ_MS) {
    lastRead = millis();
    bool pressed = digitalRead(BUTTON_PIN) == HIGH;
    if (pressed && !wasPressed && pattern == nullptr) {
      Serial.println("press");
      play(DOUBLE_MS);
    }
    wasPressed = pressed;
  }
  update();
}
