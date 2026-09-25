#define MOTOR_PIN 3   // Vibration motor on D3

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);   // D3 output for motor (PWM for speed)
  Serial.begin(9600);
  Serial.println("Vibration motor program started");
}

void loop() {
  // Method 1: digitalWrite (on/off only)
  digitalWrite(MOTOR_PIN, HIGH);   // Vibrate
  Serial.println("Motor vibrating...");
  delay(500);
  
  digitalWrite(MOTOR_PIN, LOW);    // Stop
  Serial.println("Motor stopped");
  delay(500);
  
  // Method 2: analogWrite for speed (optional, uncomment to try)
  // analogWrite(MOTOR_PIN, 128);   // 50% duty: may not start from rest
  // delay(500);
  // analogWrite(MOTOR_PIN, 255);   // 100% speed
  // delay(500);
  // analogWrite(MOTOR_PIN, 0);     // Stop
  // delay(500);
}
