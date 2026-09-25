#define KNOCK_PIN 2    // Knock sensor on D2
#define LED_PIN 13     // LED on D13

void setup() {
  pinMode(KNOCK_PIN, INPUT);   // D2 input, read knock sensor
  pinMode(LED_PIN, OUTPUT);    // D13 output, control LED
}

void loop() {
  int state = digitalRead(KNOCK_PIN);   // HIGH for a moment on each knock

  if (state == HIGH) {
    digitalWrite(LED_PIN, HIGH);   // Knock detected: LED on
    delay(100);                    // long enough to see
  } else {
    digitalWrite(LED_PIN, LOW);    // No knock: LED off
  }
}
