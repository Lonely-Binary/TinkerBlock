// Steam sensor on A0; higher value = more steam/humidity
#define STEAM_PIN A0

void setup() {
  Serial.begin(9600);   // Baud 9600 for steam value
  Serial.println("Steam sensor program started");
}

void loop() {
  int val = analogRead(STEAM_PIN);   // Read A0, 0-1023
  Serial.print("Steam: ");
  Serial.println(val);   // Higher usually = more steam/humidity
  delay(200);   // Update every 200 ms
}
