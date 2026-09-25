/*
  Vibration Motor - kick-start for low levels            TK30 / /p/tk30

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: an Uno's 5V, the 5V pin of an ESP32 or ESP32-S3
              board on USB, a Pico's VBUS (3V3 works as well)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> a PWM pin: D9 on an Uno, GPIO 4 on an ESP32 or
              ESP32-S3, GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed. ESP32 boards need board package 2.0 or later.
*/

// A PWM pin.
// Uno: 9. ESP32: 4. ESP32-S3: 4. Pico: 15.
const int MOTOR_PIN = 9;

const int KICK_MS = 50;       // full power first: a guess, tune it
bool running = false;

// Any strength, 0 to 255. From rest, kick it first.
void setLevel(int level) {
  if (level > 0 && !running) {
    analogWrite(MOTOR_PIN, 255);
    delay(KICK_MS);
  }
  analogWrite(MOTOR_PIN, level);
  running = level > 0;
}

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  const int tries[] = {150, 100, 70};
  for (int level : tries) {
    Serial.print("level ");
    Serial.println(level);
    setLevel(level);
    delay(1500);
    setLevel(0);
    delay(1000);
  }
}
