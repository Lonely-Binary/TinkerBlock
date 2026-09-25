// Option 2: Potentiometer → buzzer frequency
#define POT_PIN A0
#define BUZZER_PIN 3

void setup() {
  Serial.begin(9600);
}

void loop() {
  int potVal = analogRead(POT_PIN);
  int frequency = map(potVal, 0, 1023, 200, 2000);   // Map to 200-2000 Hz
  tone(BUZZER_PIN, frequency);
  Serial.print("Pot: ");
  Serial.print(potVal);
  Serial.print(" | Freq: ");
  Serial.println(frequency);
  delay(50);
}
