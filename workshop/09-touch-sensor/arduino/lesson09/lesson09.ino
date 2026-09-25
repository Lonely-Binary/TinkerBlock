// Touch sensor on D2, LED on D13: output HIGH when metal pad is touched, LED on
#define TOUCH_PIN 2
#define LED_PIN 13

void setup() {
  pinMode(TOUCH_PIN, INPUT);   // D2 read touch sensor
  pinMode(LED_PIN, OUTPUT);    // D13 control LED
}

void loop() {
  int state = digitalRead(TOUCH_PIN);   // TK43 is HIGH when touched, LOW when not

  if (state == HIGH) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(100);   // debounce
}
