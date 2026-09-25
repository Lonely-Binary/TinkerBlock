// RGB LED: red(D3), green(D5), blue(D6), one color at a time then all off
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

void setup() {
  pinMode(RED_PIN, OUTPUT);     // red / green / blue all outputs
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("RGB LED started");
}

void loop() {
  digitalWrite(RED_PIN, HIGH);   // red only
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  Serial.println("Red");
  delay(1000);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);  // green only
  digitalWrite(BLUE_PIN, LOW);
  Serial.println("Green");
  delay(1000);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);  // blue only
  Serial.println("Blue");
  delay(1000);
  digitalWrite(RED_PIN, LOW);     // all off
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  Serial.println("Off");
  delay(1000);
}
