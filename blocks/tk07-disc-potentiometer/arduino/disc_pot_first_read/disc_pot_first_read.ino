/*
  Disc Potentiometer - first read                        TK07 / /p/tk07

  Wiring. Count from the square pad on the TinkerBlock board, wheel
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (at one stop, SIGNAL gives your pin whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int POT_PIN = 4;

// What analogRead returns at full scale on your board.
// Uno: 1023. ESP32, ESP32-S3: 4095. Pico: 1023.
const int ADC_MAX = 4095;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int reading = analogRead(POT_PIN);   // 0 at one stop, ADC_MAX at the other

  Serial.print(reading);
  Serial.print(" of ");
  Serial.println(ADC_MAX);
  delay(200);                          // five lines a second
}
