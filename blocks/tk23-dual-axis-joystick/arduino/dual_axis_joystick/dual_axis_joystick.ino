// Uno: A0 A1 2. ESP32: 34 35 25. ESP32-S3: 4 5 7. Pico: 26 27 15.
const int X_PIN = 4, Y_PIN = 5, SW_PIN = 7;
int centreX, centreY;

void setup() {
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT);             // R10 on the board pulls it down
  delay(200);                         // hands off the stick
  centreX = analogRead(X_PIN);        // where this stick rests
  centreY = analogRead(Y_PIN);
}

void loop() {
  Serial.print("X ");
  Serial.print(analogRead(X_PIN) - centreX);
  Serial.print("  Y ");
  Serial.print(analogRead(Y_PIN) - centreY);
  Serial.print("  SW ");
  Serial.println(digitalRead(SW_PIN)); // 1 while pressed
  delay(200);
}
