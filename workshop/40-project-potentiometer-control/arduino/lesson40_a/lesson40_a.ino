// Option 1: Potentiometer → LED brightness
#define POT_PIN A0
#define LED_PIN 3    // PWM pin

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int potVal = analogRead(POT_PIN);
  int brightness = map(potVal, 0, 1023, 0, 255);   // Map to 0-255
  analogWrite(LED_PIN, brightness);
  Serial.print("Pot: ");
  Serial.print(potVal);
  Serial.print(" | Brightness: ");
  Serial.println(brightness);
  delay(50);
}
