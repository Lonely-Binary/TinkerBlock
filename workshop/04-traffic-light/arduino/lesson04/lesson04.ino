// Traffic light: red(D2), yellow(D3), green(D4) on in sequence, others off
#define RED_PIN 2      // red to D2
#define YELLOW_PIN 3   // yellow to D3
#define GREEN_PIN 4    // green to D4

void setup() {
  pinMode(RED_PIN, OUTPUT);     // red / yellow / green all outputs
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  // Red on 5 s, yellow and green off
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  delay(5000);

  // Green on 5 s, red and yellow off
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  delay(5000);

  // Yellow on 2 s, red and green off
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  delay(2000);
}
