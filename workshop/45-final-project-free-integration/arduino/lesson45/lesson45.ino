// Example: Smart lamp — potentiometer for brightness, button to change color (white/red/green/blue)
#define POT_PIN A0
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6
#define BUTTON_PIN 2

int brightness = 128;
int colorMode = 0;  // 0=white, 1=red, 2=green, 3=blue

void setup() {
  pinMode(RED_PIN, OUTPUT);     // RGB PWM
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // Button to change color
}

void loop() {
  // Read potentiometer for brightness
  int potVal = analogRead(POT_PIN);
  brightness = map(potVal, 0, 1023, 0, 255);
  
  // Button: change color
  if (digitalRead(BUTTON_PIN) == LOW) {
    colorMode = (colorMode + 1) % 4;
    delay(200);
  }
  
  // Set RGB by color mode
  switch (colorMode) {
    case 0:  // White
      analogWrite(RED_PIN, brightness);
      analogWrite(GREEN_PIN, brightness);
      analogWrite(BLUE_PIN, brightness);
      break;
    case 1:  // Red
      analogWrite(RED_PIN, brightness);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      break;
    case 2:  // Green
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, brightness);
      analogWrite(BLUE_PIN, 0);
      break;
    case 3:  // Blue
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, brightness);
      break;
  }
  
  delay(50);
}
