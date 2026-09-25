#define TEMP_PIN A0   // Temperature sensor on A0

void setup() {
  Serial.begin(9600);   // Start serial, baud 9600
  Serial.println("NTC Thermistor program started");
}

void loop() {
  int val = analogRead(TEMP_PIN);   // Read A0, store in val (0-1023)
  
  // Simple conversion: map analog value (0-1023) to voltage (0-5V)
  float voltage = val * (5.0 / 1023.0);
  
  // Print raw value and voltage
  Serial.print("Analog: ");
  Serial.print(val);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);   // 2 decimal places
  Serial.println("V");
  
  delay(500);   // 500 ms
}
