/*
  Steam Sensor - first reading                          TK65 / /p/tk65

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno.
              Your board's logic voltage: a wet board can put
              VCC itself on the pin.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32,
              GPIO 4 on an ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int STEAM_PIN = 4;

void setup() {
  Serial.begin(115200);
  // No pinMode: an analog pin needs none to be read.
}

void loop() {
  // 0 on a dry board. Water across the loops raises it:
  // up to 1023 on an Uno or a Pico, 4095 on an ESP32 or S3.
  int reading = analogRead(STEAM_PIN);

  Serial.print("reading ");
  Serial.println(reading);

  delay(250);
}
