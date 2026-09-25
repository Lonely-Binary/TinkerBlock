// Piezo sensor on A0; pressure/vibration changes output voltage, ADC 0-1023
#define PIEZO_PIN A0

void setup() {
  Serial.begin(9600);
  Serial.println("Piezo sensor program started");
}

void loop() {
  int val = analogRead(PIEZO_PIN);   // Read A0; more pressure/vibration = higher value
  Serial.print("Pressure: ");
  Serial.println(val);
  delay(100);   // Poll every 100 ms
}
