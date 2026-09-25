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
  if (val >= 100 && val <= 130) {
    Serial.println("Up");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 270 && val <= 290) {
    Serial.println("Left");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 450 && val <= 470) {
    Serial.println("Down");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 1000 && val <= 1024) {
    Serial.println("Right");
    digitalWrite(LED_PIN, HIGH);
  } else if (val >= 180 && val <= 190) {
    Serial.println("Press");
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("Center");
    digitalWrite(LED_PIN, LOW);
  }
  delay(200);   // Poll interval
}
