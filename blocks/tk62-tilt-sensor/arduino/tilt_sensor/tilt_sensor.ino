// Pin number: change this to match your wiring
#define TILT_PIN 2     // Arduino digital pin connected to SIGNAL (e.g. D2)
#define LED_PIN 13     // LED pin (Arduino built-in LED on pin 13, or external LED)

void setup() {
  // Initialize pin modes
  pinMode(TILT_PIN, INPUT);   // Set tilt sensor pin as input (to read detection state)
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output (to control LED on/off)
  
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
  
  Serial.println("Tilt sensor program started");
  Serial.println("LED on when tilt detected, LED off when level");
}

void loop() {
  // Read tilt sensor state
  int tiltState = digitalRead(TILT_PIN);  // Read sensor pin level: HIGH(1)=tilt, LOW(0)=level
  
  // Control LED based on detection state
  if (tiltState == HIGH) {
    // Tilt detected: LED on
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Tilt detected - LED on");
  } else {
    // Level state: LED off
    digitalWrite(LED_PIN, LOW);
    Serial.println("Level state - LED off");
  }
  
  delay(100);  // Brief delay to avoid reading too fast
}
