// Dual LEDs: warm D5, cool D3; PWM each; mix ratio = color temperature
// VCC -> 5V. D2 cannot do PWM on an Uno, so warm is on D5.
#define WARM_PIN 5   // 3000K warm white
#define COLD_PIN 3   // 6500K cool white

// On 5V an LED held fully on runs past its rating: stop at 170.
#define FULL 170

void setup() {
  pinMode(WARM_PIN, OUTPUT);   // D5 warm PWM
  digitalWrite(WARM_PIN, LOW);
  pinMode(COLD_PIN, OUTPUT);   // D3 cool PWM
  digitalWrite(COLD_PIN, LOW);
  Serial.begin(9600);
  Serial.println("Extreme bright dual LED program started");
}

void loop() {
  analogWrite(WARM_PIN, FULL);      // Warm only
  analogWrite(COLD_PIN, 0);
  Serial.println("Warm white");
  delay(2000);
  analogWrite(WARM_PIN, 0);         // Cool only
  analogWrite(COLD_PIN, FULL);
  Serial.println("Cool white");
  delay(2000);
  analogWrite(WARM_PIN, FULL / 2);  // Both half; mixed
  analogWrite(COLD_PIN, FULL / 2);
  Serial.println("Mixed");
  delay(2000);
}
