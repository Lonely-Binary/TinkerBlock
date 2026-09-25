// Uno: A0 A1 A2 2. ESP32: 34 35 32 25. ESP32-S3: 4 5 6 7. Pico: 26 27 28 15.
const int X_PIN = 4, Y_PIN = 5, BTNS_PIN = 6, KEY_PIN = 7;
const float FULL_SCALE_MV = 5000.0;   // Uno and Pico only. Pico: 3300.

const char* BUTTON_NAME[] = {"U11", "U10", "U5", "U7", "U8", "U6"};
const int LIMIT_MV[] = {2475, 1375, 963, 743, 605, 275};

float readMv(int pin) {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(pin);
#else
  return analogRead(pin) * FULL_SCALE_MV / 1023.0;
#endif
}

void setup() {
  Serial.begin(115200);
  pinMode(KEY_PIN, INPUT);            // R1 on the board pulls it down
}

void loop() {
  float mv = readMv(BTNS_PIN);
  int b = -1;
  for (int i = 0; i < 6 && b < 0; i++) {
    if (mv > LIMIT_MV[i]) b = i;
  }
  Serial.print("X ");
  Serial.print(analogRead(X_PIN));
  Serial.print("  Y ");
  Serial.print(analogRead(Y_PIN));
  Serial.print("  KEY ");
  Serial.print(digitalRead(KEY_PIN)); // 1 while clicked
  Serial.print("  ");
  Serial.println(b < 0 ? "none" : BUTTON_NAME[b]);
  delay(200);
}
