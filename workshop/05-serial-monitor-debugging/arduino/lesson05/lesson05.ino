// Serial debug: read button and print state to serial, and control LED; baud 9600
#define BUTTON_PIN 2   // button to D2
#define LED_PIN 13     // LED to D13

void setup() {
  pinMode(BUTTON_PIN, INPUT);   // D2 read button
  pinMode(LED_PIN, OUTPUT);     // D13 control LED
  Serial.begin(9600);           // serial baud 9600, must match monitor
  Serial.println("Program started");  // printed once after power-up
}

void loop() {
  int state = digitalRead(BUTTON_PIN);

  Serial.print("Button: ");   // print prefix first, no newline
  if (state == HIGH) {
    Serial.println("pressed");   // then print state and newline
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("released");
    digitalWrite(LED_PIN, LOW);
  }
  delay(200);   // lower refresh rate so you can read it
}
