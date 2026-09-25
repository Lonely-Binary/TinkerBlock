/*
  Dual Bright LEDs - the first light                     TK95 / /p/tk95

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V (VBUS on a Pico). It only feeds the LEDs, so 5V is
              safe on a 3.3 V board. Never the Uno's 3.3V pin.
    3000K  -> D9, GPIO 25, GPIO 4 or GP14   (a PWM pin)
    6500K  -> D10, GPIO 26, GPIO 5 or GP15  (a PWM pin)

  Uno, ESP32, ESP32-S3, Pico, in that order.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
    Serial Monitor                115200

  Do not look into the LEDs from close up.
*/

// The pins 3000K and 6500K are wired to.
// Uno: 9. ESP32: 25. ESP32-S3: 4. Pico: 14.
const int WARM_PIN = 4;
// Uno: 10. ESP32: 26. ESP32-S3: 5. Pico: 15.
const int COOL_PIN = 5;

// With VCC on 5V an LED held fully on runs past its rating, so the
// sketch stops at two thirds. With VCC on 3V3, 255 is fine.
const int MAX_DUTY = 170;

void show(int warm, int cool, const char *what) {
  analogWrite(WARM_PIN, warm);
  analogWrite(COOL_PIN, cool);
  Serial.println(what);
  delay(2000);
}

void setup() {
  // LOW first: nothing on the board holds the transistors off.
  pinMode(WARM_PIN, OUTPUT);
  digitalWrite(WARM_PIN, LOW);
  pinMode(COOL_PIN, OUTPUT);
  digitalWrite(COOL_PIN, LOW);
  Serial.begin(115200);
}

void loop() {
  show(MAX_DUTY, 0, "warm: 3000K");
  show(0, MAX_DUTY, "cool: 6500K");
  show(MAX_DUTY / 2, MAX_DUTY / 2, "both, half each: about 4100 K");
  show(0, 0, "off");
}
