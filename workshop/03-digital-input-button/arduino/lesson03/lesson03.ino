// Button on D2, LED on D13: read button state, on when pressed, off when released
#define BUTTON_PIN 2    // button SIGNAL to D2
#define LED_PIN 13      // LED SIGNAL to D13

void setup() {
  pinMode(BUTTON_PIN, INPUT);   // D2 as input, to read button
  pinMode(LED_PIN, OUTPUT);     // D13 as output, to control LED
}

void loop() {
  int state = digitalRead(BUTTON_PIN);   // TK04 is HIGH when pressed, LOW when released

  if (state == HIGH) {                   // condition true means button pressed
    digitalWrite(LED_PIN, HIGH);          // pressed: LED on
  } else {
    digitalWrite(LED_PIN, LOW);           // released: LED off
  }
  delay(100);   // short delay to reduce mis-triggers from mechanical bounce (simple debounce)
}
