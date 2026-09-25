/*
  XL LED - blinking without delay()                       TK01 / /p/tk01

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
const unsigned long INTERVAL_MS = 500;   // on for 500 ms, off for 500 ms

unsigned long lastChange = 0;
bool ledOn = false;
unsigned long passes = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  // Has half a second gone by since the LED last changed?
  if (now - lastChange >= INTERVAL_MS) {
    lastChange = now;
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);

    if (ledOn) {                    // once a second, report and reset
      Serial.print("loop() ran ");
      Serial.print(passes);
      Serial.println(" times in the last second");
      passes = 0;
    }
  }

  passes++;
  // Anything else goes here: read a button, a sensor, the serial port.
  // None of it waits for the LED.
}
