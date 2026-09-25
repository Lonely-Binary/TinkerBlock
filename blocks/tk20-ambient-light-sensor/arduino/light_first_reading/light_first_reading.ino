/*
  Ambient Light Sensor - first reading                   TK20 / /p/tk20

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (bright light takes SIGNAL up towards VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int LIGHT_PIN = A0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int count = analogRead(LIGHT_PIN);   // more light, higher

  Serial.print("count ");
  Serial.println(count);
  delay(500);                          // twice a second
}
