// DIG_PIN, then ANA_PIN.
// Uno: 2 and A0. ESP32: 25 and 34. ESP32-S3: 7 and 4. Pico: 15 and 26.
const int DIG_PIN = 2;
const int ANA_PIN = A0;

void setup() {
  Serial.begin(115200);
  pinMode(DIG_PIN, INPUT);          // the block has its own pull-up
}

void loop() {
  Serial.print("DIG ");
  Serial.print(digitalRead(DIG_PIN));   // the decision
  Serial.print("   ANA ");
  Serial.println(analogRead(ANA_PIN));  // the number behind it
  delay(250);
}
