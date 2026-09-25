#define BUZZER_PIN 3   // Buzzer on D3

void setup() {
  Serial.begin(9600);   // Optional: print note names to serial
  Serial.println("Passive buzzer program started");
}

void loop() {
  // Play Do (262 Hz)
  tone(BUZZER_PIN, 262);   // Play 262 Hz
  Serial.println("Do");
  delay(500);
  
  // Play Re (294 Hz)
  tone(BUZZER_PIN, 294);   // Re
  Serial.println("Re");
  delay(500);
  tone(BUZZER_PIN, 330);   // Mi
  Serial.println("Mi");
  delay(500);
  noTone(BUZZER_PIN);      // Stop sound
  delay(1000);             // Pause 1 s before repeat
}
