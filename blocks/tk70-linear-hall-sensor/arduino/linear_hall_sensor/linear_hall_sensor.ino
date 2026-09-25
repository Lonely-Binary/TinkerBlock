// Pin number: change this to match your wiring
#define HALL_PIN A0  // Arduino analog input pin connected to SIGNAL (e.g. A0)

void setup() {
  // Initialize pin mode
  pinMode(HALL_PIN, INPUT);   // Set Hall sensor pin as input (to read analog value)
  
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
  
  Serial.println("Linear Hall sensor program started");
  Serial.println("Reading magnetic field strength value and output via serial");
}

void loop() {
  // Read Hall sensor analog value (0-1023)
  int sensorValue = analogRead(HALL_PIN);  // Read sensor pin analog value: 0=no magnetic field, 1023=strong magnetic field
  
  // Convert analog value to voltage value (0-5V)
  float voltage = sensorValue * (5.0 / 1023.0);
  
  // Output strength value
  Serial.print("Magnetic field strength: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.println("V");
  
  delay(100);  // Brief delay to avoid reading too fast
}
