/*
  Analog to Digital Signal - first read                  TK29 / /p/tk29

  A TK08 Rotary Potentiometer, or any analog block, pushed into IN,
  parts facing the same way. Slide switch at H.

  Wiring, OUT to your board. Count from the square pad, parts up,
  OUT at the bottom:

    GND -> GND
    VCC -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
           (DIG is pulled up to VCC)
    DIG -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 7 on an
           ESP32-S3, GP15 on a Raspberry Pi Pico
    ANA -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
           ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// DIG_PIN, then ANA_PIN.
// Uno: 2 and A0. ESP32: 25 and 34. ESP32-S3: 7 and 4. Pico: 15 and 26.
const int DIG_PIN = 2;
const int ANA_PIN = A0;

void setup() {
  Serial.begin(115200);
  pinMode(DIG_PIN, INPUT);          // the block has its own pull-up
}

void loop() {
  int dig = digitalRead(DIG_PIN);   // the decision: 1 or 0
  int ana = analogRead(ANA_PIN);    // the number it was made from

  Serial.print("DIG ");
  Serial.print(dig);
  Serial.print("   ANA ");
  Serial.println(ana);
  delay(250);                       // four times a second
}
