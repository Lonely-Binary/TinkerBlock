/*
  Hall Effect Sensor - a door alarm                      TK18 / /p/tk18

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (with no magnet, SIGNAL sits at whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  The block on the frame, a magnet on the door facing the small
  chip at the top left of the block.

  A TK01 XL LED on a second pin is the alarm (optional):
    GND    -> GND
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32, GPIO 5 on an
              ESP32-S3, GP14 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO number the Hall sensor's SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int HALL_PIN = 4;
// The GPIO number the TK01's SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 14.
const int LED_PIN = 5;
const unsigned long ALARM_AFTER_MS = 10000;

bool wasShut;
bool alarmOn = false;
unsigned long openedAt;

bool doorShut() {
  return digitalRead(HALL_PIN) == LOW;   // magnet near: LOW
}

void setup() {
  Serial.begin(115200);
  pinMode(HALL_PIN, INPUT);     // the chip drives SIGNAL both ways
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  wasShut = doorShut();
  openedAt = millis();
}

void loop() {
  bool shut = doorShut();
  unsigned long now = millis();

  if (!shut && wasShut) {       // it has just opened: start the clock
    openedAt = now;
    Serial.println("opened");
  }
  if (shut && !wasShut) {
    Serial.println("shut");
  }
  wasShut = shut;

  bool alarm = !shut && now - openedAt >= ALARM_AFTER_MS;
  if (alarm != alarmOn) {
    alarmOn = alarm;
    digitalWrite(LED_PIN, alarm ? HIGH : LOW);
    Serial.println(alarm ? "left open: alarm" : "alarm off");
  }
}
