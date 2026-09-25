#define RED_PIN 3      // Red on D3
#define GREEN_PIN 5    // Green on D5
#define BLUE_PIN 6     // Blue on D6

void setup() {
  pinMode(RED_PIN, OUTPUT);     // All three PWM outputs
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("RGB color mixing program started");
}

void loop() {
  // Red (red only)
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
  Serial.println("Red");
  delay(1000);
  
  // Green (green only)
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 0);
  Serial.println("Green");
  delay(1000);
  
  // Blue (blue only)
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 255);
  Serial.println("Blue");
  delay(1000);
  
  // White (all max)
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);
  Serial.println("White");
  delay(1000);
  
  // Yellow (red + green)
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 0);
  Serial.println("Yellow");
  delay(1000);
  
  // Purple (red + blue)
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 255);
  Serial.println("Purple");
  delay(1000);
  
  // Cyan (green + blue)
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);
  Serial.println("Cyan");
  delay(1000);
}
