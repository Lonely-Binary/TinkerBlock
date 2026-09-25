/*
  XL LED - first blink                                   TK01 / /p/tk01

  Wiring. Count from the square pad on the TinkerBlock board, LED side
  up, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32 or ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO number SIGNAL is wired to. Uno: 9. ESP32, ESP32-S3: 4. Pico: 15.
const int LED_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);     // without this, HIGH is only a weak pull-up
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // the pin supplies the LED's current
  Serial.println("on");
  delay(500);

  digitalWrite(LED_PIN, LOW);   // 0 V on the anode, nothing flows
  Serial.println("off");
  delay(500);
}
