/*
  Vibration Motor - strength with analogWrite           TK30 / /p/tk30

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: an Uno's 5V, the 5V pin of an ESP32 or ESP32-S3
              board on USB, a Pico's VBUS (3V3 works as well)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> a PWM pin: D9 on an Uno, GPIO 4 on an ESP32 or
              ESP32-S3, GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed. ESP32 boards need board package 2.0 or later.
*/

// A PWM pin.
// Uno: 9. ESP32: 4. ESP32-S3: 4. Pico: 15.
const int MOTOR_PIN = 9;

// 0 is off, 255 is full. From full to below the start line.
const int LEVELS[] = {255, 200, 150, 100};

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  for (int level : LEVELS) {
    Serial.print("level ");
    Serial.println(level);
    analogWrite(MOTOR_PIN, level);   // from rest, every time
    delay(1000);
    analogWrite(MOTOR_PIN, 0);       // off, and let it stop
    delay(1000);
  }
}
