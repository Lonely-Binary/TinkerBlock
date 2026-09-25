// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int BUTTON_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);   // the block has its own pull-down
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH) {   // HIGH is pressed
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
  delay(200);
}
