/*
  Dual Bright LEDs - warm to cool at one brightness      TK95 / /p/tk95

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

// The mix's colour temperature, estimated by averaging the two
// LEDs in mireds (a million over the kelvin). An approximation.
long mixKelvin(int coolPercent) {
  float s = coolPercent / 100.0;
  float mired = (1 - s) * (1e6 / 3000) + s * (1e6 / 6500);
  return lround(1e6 / mired / 100) * 100;
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
  // The two duties always add up to MAX_DUTY: the brightness stays,
  // the share moves from warm to cool.
  for (int p = 0; p <= 100; p += 10) {
    int cool = (long)MAX_DUTY * p / 100;
    int warm = MAX_DUTY - cool;
    analogWrite(WARM_PIN, warm);
    analogWrite(COOL_PIN, cool);
    Serial.print("cool ");
    Serial.print(p);
    Serial.print(" %: warm ");
    Serial.print(warm);
    Serial.print(", cool ");
    Serial.print(cool);
    Serial.print(", about ");
    Serial.print(mixKelvin(p));
    Serial.println(" K");
    delay(1500);
  }
}
