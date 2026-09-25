// Voltage measurement: A0 reads divided voltage; multiply by ratio for actual (e.g. 0-25V)
#define VOLTAGE_PIN A0
const float DIVIDER_RATIO = 5.0;   // Divider ratio to match module (e.g. 5:1 => actual = read × 5)

void setup() {
  Serial.begin(9600);
  Serial.println("Voltmeter program started");
}

void loop() {
  int val = analogRead(VOLTAGE_PIN);   // A0, 0-1023
  float voltage = val * (5.0 / 1023.0);        // Pin voltage 0-5V
  float actualVoltage = voltage * DIVIDER_RATIO;   // Actual measured voltage
  Serial.print("Voltage: ");
  Serial.print(actualVoltage, 2);
  Serial.println("V");
  delay(500);   // Update every 0.5 s
}
