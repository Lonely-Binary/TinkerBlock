#define POT_PIN A0      // Potentiometer on A0
#define BUTTON_PIN 2    // Button on D2
#define LED_PIN 13      // LED on D13

void setup() {
  pinMode(POT_PIN, INPUT);     // A0 input for potentiometer (analog doesn't need pinMode, but OK to write)
  pinMode(BUTTON_PIN, INPUT);  // D2 input for button
  pinMode(LED_PIN, OUTPUT);    // D13 output for LED
  
  Serial.begin(9600);   // Start serial, baud 9600
  Serial.println("Multiple sensors program started");
}

void loop() {
  int potVal = analogRead(POT_PIN);           // Read potentiometer, store in potVal
  int buttonState = digitalRead(BUTTON_PIN);  // Read button, store in buttonState
  
  // Print both to serial
  Serial.print("Pot: ");
  Serial.print(potVal);
  Serial.print(" | Button: ");
  if (buttonState == HIGH) {
    Serial.println("Pressed");
  } else {
    Serial.println("Released");
  }
  
  // Control LED by button
  if (buttonState == HIGH) {
    digitalWrite(LED_PIN, HIGH);   // Pressed: LED on
  } else {
    digitalWrite(LED_PIN, LOW);    // Released: LED off
  }
  
  delay(200);   // 200 ms
}
