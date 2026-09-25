#define POT_PIN A0   // Potentiometer on A0
#define LED_PIN 9    // XL LED on D9 (PWM pin)

void setup() {
  pinMode(LED_PIN, OUTPUT);   // D9 output for LED (optional)
  Serial.begin(9600);         // Serial baud 9600 for printing potentiometer value
  Serial.println("Potentiometer program started");
}

void loop() {
  int val = analogRead(POT_PIN);   // Read A0, store in val (0-1023)
  
  // Use potentiometer value to control D9 LED brightness (map 0-1023 to 0-255)
  int brightness = val * 255L / 1023;
  analogWrite(LED_PIN, brightness);
  
  Serial.print("Potentiometer: ");
  Serial.println(val);   // Print to serial for observing knob change
  
  delay(200);   // Lower refresh rate for easier observation
}
