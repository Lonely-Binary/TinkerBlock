// Dual LEDs: warm D2, cool D3; PWM each; mix ratio = color temperature
#define WARM_PIN 2   // 3000K warm white
#define COLD_PIN 3   // 6500K cool white

void setup() {
  pinMode(WARM_PIN, OUTPUT);   // D2 warm PWM
  pinMode(COLD_PIN, OUTPUT);   // D3 cool PWM
  Serial.begin(9600);
  Serial.println("Extreme bright dual LED program started");
}

void loop() {
  analogWrite(WARM_PIN, 255);   // Warm only
  analogWrite(COLD_PIN, 0);
  Serial.println("Warm white");
  delay(2000);
  analogWrite(WARM_PIN, 0);    // Cool only
  analogWrite(COLD_PIN, 255);
  Serial.println("Cool white");
  delay(2000);
  analogWrite(WARM_PIN, 128);   // Both half; mixed
  analogWrite(COLD_PIN, 128);
  Serial.println("Mixed");
  delay(2000);
}
