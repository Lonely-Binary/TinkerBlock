// Joystick: up/down = LED, left/right = pitch, press = stop buzzer
#define JOYSTICK_PIN A0
#define LED_PIN 13
#define BUZZER_PIN 3

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Joystick control started");
}

void loop() {
  int val = analogRead(JOYSTICK_PIN);   // Different ranges = direction
  // Up about 114, press 186, left 279, down 455, right 1023, and 0
  // with nothing pressed. Each range runs halfway to its neighbours.
  if (val >= 57 && val < 150) {
    // Up: LED on
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Up - LED on");
  } else if (val >= 367 && val < 739) {
    // Down: LED off
    digitalWrite(LED_PIN, LOW);
    Serial.println("Down - LED off");
  } else if (val >= 233 && val < 367) {
    // Left: low tone
    tone(BUZZER_PIN, 262);
    Serial.println("Left - low");
  } else if (val >= 739) {
    // Right: high tone
    tone(BUZZER_PIN, 523);
    Serial.println("Right - high");
  } else if (val >= 150 && val < 233) {
    // Press: stop buzzer
    noTone(BUZZER_PIN);
    Serial.println("Press - stop");
  } else {
    noTone(BUZZER_PIN);   // Center/other: stop buzzer
  }
  delay(200);   // Poll interval
}
