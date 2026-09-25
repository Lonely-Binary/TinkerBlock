// Collision sensor on D2, LED on D13: SIGNAL is HIGH while the lever is pushed in
#define COLLISION_PIN 2
#define LED_PIN 13

void setup() {
  pinMode(COLLISION_PIN, INPUT);   // D2 read collision sensor
  pinMode(LED_PIN, OUTPUT);        // D13 control LED
}

void loop() {
  int state = digitalRead(COLLISION_PIN);   // HIGH while the lever is in

  if (state == HIGH) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(50);   // short delay is enough since we read often
}
