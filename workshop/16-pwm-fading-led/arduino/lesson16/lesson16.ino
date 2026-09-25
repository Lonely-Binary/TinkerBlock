#define LED_PIN 3   // LED on D3 (PWM pin)

void setup() {
  pinMode(LED_PIN, OUTPUT);   // D3 output for LED
}

void loop() {
  // Dim to bright (0 → 255)
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(LED_PIN, brightness);   // Set brightness (0=dim, 255=bright)
    delay(10);   // 10 ms for smoother fade
  }
  
  // Bright to dim (255 → 0)
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(LED_PIN, brightness);   // Output current brightness
    delay(10);   // 10 ms per step for smoother fade
  }
}
