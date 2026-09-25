#define BUZZER_PIN 3   // Buzzer on D3
const int DURATION = 500;   // Duration in milliseconds

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);   // D3 output for buzzer
  Serial.begin(9600);
  Serial.println("Active buzzer program started");
}

void loop() {
  // Buzzer on
  digitalWrite(BUZZER_PIN, HIGH);
  Serial.println("Buzzer on");
  delay(DURATION);
  
  // Buzzer off
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Buzzer off");
  delay(DURATION);
}
