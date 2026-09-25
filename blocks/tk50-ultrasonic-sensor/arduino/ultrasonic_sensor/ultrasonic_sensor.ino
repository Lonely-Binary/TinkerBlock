#define TRIG 9
#define ECHO 10

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);
}

void loop() {
  // The supported library holds TRIG high for 500 us, so this does too.
  // Every HC-SR04 example uses 10 us.
  digitalWrite(TRIG, HIGH); delayMicroseconds(500);
  digitalWrite(TRIG, LOW);

  // Time out at ~4 m of round trip. Without a limit, a missing echo blocks
  // for a full second and then reads as a valid measurement of nothing.
  long us = pulseIn(ECHO, HIGH, 25000UL);

  if (us == 0) {
    Serial.println("no echo");        // absorbed, angled away, or out of range
  } else {
    Serial.print((us / 2.0) * 0.0343, 1);   // cm at about 20 C
    Serial.println(" cm");
  }

  delay(100);   // A measurement cycle is 50 ms. Let the last burst die away.
}
