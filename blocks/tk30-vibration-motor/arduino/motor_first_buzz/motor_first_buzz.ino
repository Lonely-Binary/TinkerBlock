/*
  Vibration Motor - first buzz                          TK30 / /p/tk30

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: an Uno's 5V, the 5V pin of an ESP32 or ESP32-S3
              board on USB, a Pico's VBUS (3V3 works as well)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32 or ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 4. Pico: 15.
const int MOTOR_PIN = 9;

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  digitalWrite(MOTOR_PIN, HIGH);   // the MOSFET closes: buzz
  Serial.println("on");
  delay(500);

  digitalWrite(MOTOR_PIN, LOW);    // open: the motor coasts to a stop
  Serial.println("off");
  delay(500);
}
