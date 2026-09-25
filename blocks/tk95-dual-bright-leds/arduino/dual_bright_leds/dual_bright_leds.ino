// Pin number: change these to match your wiring
#define WARM_PIN 2   // Arduino PWM pin connected to 3000K (e.g. D3)
#define COLD_PIN 3   // Arduino PWM pin connected to 6500K (e.g. D5)

void setup() {
  // Set LED pins as output mode
  pinMode(WARM_PIN, OUTPUT);
  pinMode(COLD_PIN, OUTPUT);
  
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
}

void loop() {
  // Warm white (3000K brightest, 6500K off)
  analogWrite(WARM_PIN, 255);   // Warm white brightest
  analogWrite(COLD_PIN, 0);      // Cool white off
  Serial.println("Warm white");
  delay(2000);
  
  // Cool white (3000K off, 6500K brightest)
  analogWrite(WARM_PIN, 0);
  analogWrite(COLD_PIN, 255);
  Serial.println("Cool white");
  delay(2000);
  
  // Mixed light (both LEDs on, color temperature in middle)
  analogWrite(WARM_PIN, 128);   // Warm white medium brightness
  analogWrite(COLD_PIN, 128);    // Cool white medium brightness
  Serial.println("Mixed light");
  delay(2000);
}
