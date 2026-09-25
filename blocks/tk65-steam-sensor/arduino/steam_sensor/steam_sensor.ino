// Pin number: change this to match your wiring
#define STEAM_PIN A0   // Arduino analog input pin connected to SIGNAL (e.g. A0)

// Filtering and change detection variables
int lastValue = -1;  // Previous value
const int CHANGE_THRESHOLD = 10;  // Change threshold, only output if change exceeds this value

void setup() {
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
  
  Serial.println("Steam sensor program started");
  Serial.println("Reading analog value, higher value indicates higher steam concentration");
  Serial.println("Only output when value changes significantly (reduce noise interference)");
}

void loop() {
  // Multiple samples and average to reduce noise
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(STEAM_PIN);
    delayMicroseconds(100);
  }
  int steamValue = sum / 5;  // Calculate average
  
  // Only output when value changes significantly (reduce random output in stable state)
  if (lastValue == -1) {
    // First reading, output directly
    Serial.print("Steam sensor value: ");
    Serial.println(steamValue);
    lastValue = steamValue;
  } else {
    // Calculate change amount
    int change = abs(steamValue - lastValue);
    if (change >= CHANGE_THRESHOLD) {
      Serial.print("Steam sensor value: ");
      Serial.print(steamValue);
      Serial.print(" (change: ");
      Serial.print(change);
      Serial.println(")");
      lastValue = steamValue;
    }
  }
  
  // Delay 200 milliseconds to avoid output too fast
  delay(200);
}
