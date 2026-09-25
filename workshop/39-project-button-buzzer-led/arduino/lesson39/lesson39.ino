#define BUTTON_PIN 2   // Button on D2
#define BUZZER_PIN 3   // Buzzer on D3
#define LED_PIN 13     // LED on D13

void setup() {
  pinMode(BUTTON_PIN, INPUT);   // D2 read button
  pinMode(BUZZER_PIN, OUTPUT);  // D3 buzzer
  pinMode(LED_PIN, OUTPUT);     // D13 LED
}

void loop() {
  int state = digitalRead(BUTTON_PIN);   // Pressed=HIGH, released=LOW
  if (state == HIGH) {
    digitalWrite(LED_PIN, HIGH);      // Pressed: LED on
    digitalWrite(BUZZER_PIN, HIGH);   // Pressed: buzzer on
  } else {
    digitalWrite(LED_PIN, LOW);       // Released: LED off
    digitalWrite(BUZZER_PIN, LOW);    // Released: buzzer off
  }
  delay(100);   // Debounce
}
