/*
  Latching button - first read                          TK05 / /p/tk05

  Wiring. Count from the square pad on the TinkerBlock board, switch
  side up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (never 5V on a 3.3 V board: SIGNAL is VCC when on)
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
const int SWITCH_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);   // the board's pull-down holds it LOW
}

void loop() {
  if (digitalRead(SWITCH_PIN) == HIGH) {   // switch on: SIGNAL is VCC
    Serial.println("SIGNAL is HIGH: the switch is ON");
  } else {                                 // switch off: 0 V
    Serial.println("SIGNAL is LOW: the switch is OFF");
  }
  delay(500);
}
