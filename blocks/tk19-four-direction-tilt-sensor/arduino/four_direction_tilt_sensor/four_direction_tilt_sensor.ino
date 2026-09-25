// Uno: 2 3 4 5. ESP32: 25 26 27 32. ESP32-S3: 4 5 6 7. Pico: 10 11 12 13.
const int PIN_A = 4, PIN_B = 5, PIN_C = 6, PIN_D = 7;

void releaseAll() {             // every line an input with its pull-up
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_C, INPUT_PULLUP);
  pinMode(PIN_D, INPUT_PULLUP);
}

void holdLow(int pin) {         // the only way a line is ever driven
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

const char* scanTilt() {
  releaseAll();
  holdLow(PIN_A);
  delayMicroseconds(10);
  bool ab = digitalRead(PIN_B) == LOW;
  bool da = digitalRead(PIN_D) == LOW;
  releaseAll();
  holdLow(PIN_C);
  delayMicroseconds(10);
  bool bc = digitalRead(PIN_B) == LOW;
  bool cd = digitalRead(PIN_D) == LOW;
  releaseAll();
  if (ab) return "left";
  if (da) return "header";
  if (bc) return "top";
  if (cd) return "right";
  return "none";
}

void setup() {
  Serial.begin(115200);
  releaseAll();
}

void loop() {
  Serial.println(scanTilt());
  delay(200);
}
