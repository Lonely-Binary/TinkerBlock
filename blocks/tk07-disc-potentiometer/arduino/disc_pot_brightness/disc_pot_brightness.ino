/*
  Disc Potentiometer - a percentage and a brightness     TK07 / /p/tk07

  Wiring. Count from the square pad on each TinkerBlock board, header
  at the bottom. TK07 Disc Potentiometer:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (at one stop, SIGNAL gives your pin whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  TK01 XL LED: GND -> GND, both NC -> nothing, and
    SIGNAL -> D9 on an Uno, GPIO 25 on an ESP32, GPIO 5 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

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

// The pin the TK01's SIGNAL is wired to.
// Uno: 9. ESP32: 25. ESP32-S3: 5. Pico: 15.
const int LED_PIN = 5;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int reading = analogRead(POT_PIN);

  int percent = map(reading, 0, ADC_MAX, 0, 100);
  int duty = map(reading, 0, ADC_MAX, 0, 255);

  analogWrite(LED_PIN, duty);          // 0 is off, 255 is fully on

  Serial.print(percent);
  Serial.print(" %   duty ");
  Serial.println(duty);
  delay(50);
}
