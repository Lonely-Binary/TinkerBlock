// A PWM pin. Uno: 9. ESP32: 4. ESP32-S3: 4. Pico: 15.
const int MOTOR_PIN = 9;
const int KICK_MS = 50;   // full power first, so low levels start

void buzz(int level, int ms) {
  analogWrite(MOTOR_PIN, 255);      // kick: sure to start
  delay(KICK_MS);
  analogWrite(MOTOR_PIN, level);    // then the strength you want
  delay(ms);
  analogWrite(MOTOR_PIN, 0);
}

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  buzz(255, 300);   // strong
  delay(700);
  buzz(150, 300);   // gentler
  delay(700);
}
