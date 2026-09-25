#define LED_PIN 13   // LED on D13

// Custom function: blink once
void myBlink() {
  digitalWrite(LED_PIN, HIGH);   // On
  delay(200);                    // 200 ms
  digitalWrite(LED_PIN, LOW);    // Off
  delay(200);                    // 200 ms
}

void setup() {
  pinMode(LED_PIN, OUTPUT);   // D13 for LED
}

void loop() {
  myBlink();   // Call custom function: blink once
  delay(500);  // 500 ms before next blink
}
