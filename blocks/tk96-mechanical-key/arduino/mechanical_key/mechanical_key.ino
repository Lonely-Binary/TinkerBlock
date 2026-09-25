// Pin number: change this to match your wiring
#define BUTTON_PIN 2   // Arduino digital pin connected to BUTTON (e.g. D2)
#define LED_PIN 13     // LED pin (Arduino built-in LED on pin 13, or external LED)

void setup() {
  // Initialize pin modes
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Set button pin as input mode, enable internal pull-up resistor
  pinMode(LED_PIN, OUTPUT);           // Set LED pin as output mode (to control LED on/off)
  
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
  
  Serial.println("Mechanical key program started");
  Serial.println("LED on when key pressed, LED off when key released");
}

void loop() {
  // Read button state
  int buttonState = digitalRead(BUTTON_PIN);  // Read button pin level: LOW(0)=pressed, HIGH(1)=released
  
  // Control LED based on button state
  if (buttonState == LOW) {
    // Key pressed: LED on
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Key pressed - LED on");
  } else {
    // Key released: LED off
    digitalWrite(LED_PIN, LOW);
    Serial.println("Key released - LED off");
  }
  
  delay(100);  // Brief delay to avoid reading too fast
}
