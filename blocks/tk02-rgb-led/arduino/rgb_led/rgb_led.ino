// Uno: 9, 10, 11. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
const int RED_PIN = 4;
const int GREEN_PIN = 5;
const int BLUE_PIN = 6;

// 0 is off, 255 is full. Common cathode: no "255 -" anywhere.
void setColour(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  setColour(255, 0, 0);    // red
  delay(1000);
  setColour(0, 255, 0);    // green
  delay(1000);
  setColour(0, 0, 255);    // blue
  delay(1000);
}
