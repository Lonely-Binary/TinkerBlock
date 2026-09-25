/*
  Active Buzzer - beeps without delay()                 TK36 / /p/tk36

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: an Uno's 5V, the 5V pin of an ESP32 or ESP32-S3
              board on USB, a Pico's VBUS (3V3 works, quieter)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3 or ESP32, D9 on an Uno,
              GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Serial Monitor at 115200, line ending of your choice.
    No library needed.

  Type o, d, e or a in the Serial Monitor and press Enter.
*/

// The pin SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 4. Pico: 15.
const int BUZZER_PIN = 4;

// On and off times in ms, on first. A 0 ends the list.
const unsigned int patOk[] = {60, 0};
const unsigned int patDouble[] = {80, 80, 80, 0};
const unsigned int patError[] = {500, 0};
const unsigned int patAlarm[] = {150, 100, 150, 100, 150, 100, 150, 0};

const unsigned int *pattern = nullptr;  // what is playing, if anything
int step = 0;                           // which time in the list
unsigned long stepStart = 0;            // when that step began
unsigned long loops = 0;                // passes of loop() meanwhile

void play(const unsigned int *p) {
  pattern = p;
  step = 0;
  loops = 0;
  stepStart = millis();
  digitalWrite(BUZZER_PIN, HIGH);       // every pattern starts on
}

// Called on every pass of loop(). Never waits.
void updateBuzzer() {
  if (pattern == nullptr) return;
  if (millis() - stepStart < pattern[step]) return;

  step++;
  stepStart = millis();
  if (pattern[step] == 0) {             // end of the list
    digitalWrite(BUZZER_PIN, LOW);
    pattern = nullptr;
    Serial.print("done; loop() ran ");
    Serial.print(loops);
    Serial.println(" times while it played");
    return;
  }
  // Even steps are on, odd steps are off.
  digitalWrite(BUZZER_PIN, step % 2 == 0 ? HIGH : LOW);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);          // first: no pull-down on SIGNAL
  digitalWrite(BUZZER_PIN, LOW);
  Serial.begin(115200);
  Serial.println("Type o, d, e or a and press Enter.");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'o') play(patOk);
    if (c == 'd') play(patDouble);
    if (c == 'e') play(patError);
    if (c == 'a') play(patAlarm);
  }

  updateBuzzer();
  loops++;          // anything else the sketch does goes here
}
