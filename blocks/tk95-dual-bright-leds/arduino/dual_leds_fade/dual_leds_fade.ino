/*
  Dual Bright LEDs - a fade that looks even              TK95 / /p/tk95

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
*/

// The pins 3000K and 6500K are wired to.
// Uno: 9. ESP32: 25. ESP32-S3: 4. Pico: 14.
const int WARM_PIN = 4;
// Uno: 10. ESP32: 26. ESP32-S3: 5. Pico: 15.
const int COOL_PIN = 5;

// Two thirds of full: the cap for VCC on 5V.
const int MAX_DUTY = 170;
const int STEPS = 100;

// Step 0..STEPS to a duty, through a 2.2 power: the eye answers
// roughly to the logarithm of light, so equal steps of duty do not
// look equal. 2.2 is the usual rule of thumb.
int dutyFor(int step) {
  return lround(pow((float)step / STEPS, 2.2) * MAX_DUTY);
}

void fade(int pin, const char *name) {
  Serial.print(name);
  Serial.println(": up");
  for (int s = 0; s <= STEPS; s++) {
    analogWrite(pin, dutyFor(s));
    delay(20);
  }
  Serial.print(name);
  Serial.println(": down");
  for (int s = STEPS; s >= 0; s--) {
    analogWrite(pin, dutyFor(s));
    delay(20);
  }
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
  fade(WARM_PIN, "3000K");
  fade(COOL_PIN, "6500K");
}
