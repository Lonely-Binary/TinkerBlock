// ADC demo: compare analog (continuous) vs digital (high/low only)
void setup() {
  Serial.begin(9600);
  Serial.println("ADC concept demo");
}

void loop() {
  int analogVal = analogRead(A0);   // A0 via ADC → 0-1023, maps to 0V-5V
  int digitalVal = digitalRead(2);  // D2 digital: only HIGH(1) or LOW(0)
  
  Serial.print("Analog: ");
  Serial.print(analogVal);
  Serial.print(" | Digital: ");
  Serial.println(digitalVal);
  
  delay(200);   // One line every 200 ms
}
