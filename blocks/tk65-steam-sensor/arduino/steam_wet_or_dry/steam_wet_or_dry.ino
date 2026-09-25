/*
  Steam Sensor - wet or dry                             TK65 / /p/tk65

  Wiring. The same as the first reading, except VCC, which comes
  from a pin so the loops only carry current while being read.
  Count from the square pad on the TinkerBlock board, parts up,
  header at the bottom:

    GND    -> GND
    VCC    -> POWER_PIN: D7 on an Uno, GPIO 25 on an ESP32,
              GPIO 5 on an ESP32-S3, GP15 on a Raspberry Pi Pico.
              It drives HIGH at your board's logic voltage.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32,
              GPIO 4 on an ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int STEAM_PIN = 4;

// The pin that powers VCC.
// Uno: 7. ESP32: 25. ESP32-S3: 5. Pico: 15.
const int POWER_PIN = 5;

// Full scale: 4095 on an ESP32 or S3, 1023 on an Uno or a Pico.
#if defined(ARDUINO_ARCH_ESP32)
const int FULL = 4095;
#else
const int FULL = 1023;
#endif

// Wet above 30 % of full scale, dry again only below 10 %.
const int WET_AT = FULL * 30 / 100;
const int DRY_AT = FULL * 10 / 100;

// The 1 MΩ and the 100 nF take 0.1 s per time constant;
// five of them is 500 ms, within 1 % of the final value.
const unsigned long SETTLE_MS = 500;
const unsigned long PERIOD_MS = 10000;   // one reading every 10 s

bool wet = false;

void setup() {
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);          // loops off between reads
}

void loop() {
  digitalWrite(POWER_PIN, HIGH);
  delay(SETTLE_MS);
  int reading = analogRead(STEAM_PIN);
  digitalWrite(POWER_PIN, LOW);

  Serial.print("reading ");
  Serial.println(reading);

  if (!wet && reading > WET_AT) {
    wet = true;
    Serial.println("wet");
  } else if (wet && reading < DRY_AT) {
    wet = false;
    Serial.println("dry");
  }

  delay(PERIOD_MS - SETTLE_MS);
}
