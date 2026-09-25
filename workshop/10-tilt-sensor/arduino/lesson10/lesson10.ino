// Tilt sensor on D2, LED on D13: output HIGH when tilted, LED on
#define TILT_PIN 2
#define LED_PIN 13

void setup() {
  pinMode(TILT_PIN, INPUT);   // D2 read tilt sensor
  pinMode(LED_PIN, OUTPUT);   // D13 control LED
}

void loop() {
  int state = digitalRead(TILT_PIN);   // TK62 is HIGH when tilted, LOW when level

  if (state == HIGH) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(100);   // debounce
}
