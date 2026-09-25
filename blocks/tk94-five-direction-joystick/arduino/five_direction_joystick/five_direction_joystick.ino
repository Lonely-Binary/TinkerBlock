// Pin number: change this to match your wiring
#define SIGNAL_PIN A0   // Arduino analog input pin connected to SIGNAL (e.g. A0)

void setup() {
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
  
  Serial.println("Five-direction joystick program started");
  Serial.println("Reading SIGNAL pin analog value to determine direction");
}

void loop() {
  // Read SIGNAL pin analog value (0-1023)
  int signalValue = analogRead(SIGNAL_PIN);
  
  // Determine joystick direction (based on actual measured values)
  // Push up: 100-130
  // Push left: 270-290
  // Push down: 450-470
  // Push right: 1000-1024
  // Press: 180-190
  
  if (signalValue >= 100 && signalValue <= 130) {
    Serial.println("Push up");
  } else if (signalValue >= 270 && signalValue <= 290) {
    Serial.println("Push left");
  } else if (signalValue >= 450 && signalValue <= 470) {
    Serial.println("Push down");
  } else if (signalValue >= 1000 && signalValue <= 1024) {
    Serial.println("Push right");
  } else if (signalValue >= 180 && signalValue <= 190) {
    Serial.println("Press");
  } else {
    Serial.println("Center position");
  }
  
  // Print joystick status
  Serial.print("SIGNAL value: ");
  Serial.println(signalValue);
  
  delay(200);  // Wait 200 milliseconds before reading again
}
