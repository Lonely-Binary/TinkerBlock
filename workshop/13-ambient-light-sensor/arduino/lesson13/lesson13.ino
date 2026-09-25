#define LIGHT_PIN A0   // Light sensor on A0

void setup() {
  Serial.begin(9600);   // Start serial, baud 9600
  Serial.println("Ambient light sensor program started");
}

void loop() {
  int val = analogRead(LIGHT_PIN);   // Read A0, store in val (0-1023)
  
  Serial.print("Light: ");
  Serial.println(val);   // Print val (higher = brighter)
  
  delay(200);   // 200 ms
}
