// Five-direction joystick SIGNAL on A0; each direction/button = different voltage range; if-else for direction and LED
#define JOYSTICK_PIN A0
#define LED_PIN 13

void setup() {
  pinMode(LED_PIN, OUTPUT);   // D13 for LED
  Serial.begin(9600);
  Serial.println("Five-direction joystick started");
}

void loop() {
  int val = analogRead(JOYSTICK_PIN);   // A0; different ranges = direction/press
  // Up about 114, press 186, left 279, down 455, right 1023, and 0
  // with nothing pressed. Each range runs halfway to its neighbours.
  if (val >= 57 && val < 150) {
    Serial.println("Up");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 233 && val < 367) {
    Serial.println("Left");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 367 && val < 739) {
    Serial.println("Down");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 739) {
    Serial.println("Right");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 150 && val < 233) {
    Serial.println("Press");
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("Center");
    digitalWrite(LED_PIN, LOW);
  }
  delay(200);   // Poll interval
}
