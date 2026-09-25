// Latching button on D2, LED on D13: latched=HIGH LED on, unlatched=LOW LED off
#define BUTTON_PIN 2
#define LED_PIN 13

void setup() {
  pinMode(BUTTON_PIN, INPUT);   // D2 input, read latching button
  pinMode(LED_PIN, OUTPUT);     // D13 output, control LED
}

void loop() {
  int state = digitalRead(BUTTON_PIN);   // TK05 is HIGH when latched, LOW when unlatched

  if (state == HIGH) {
    digitalWrite(LED_PIN, HIGH);   // latched: LED on
  } else {
    digitalWrite(LED_PIN, LOW);    // unlatched: LED off
  }
  delay(100);   // short delay to reduce bounce
}
