/*
  Voltmeter - low-battery warning                        TK09 / /p/tk09

  Wiring. Count from the square pad on the TinkerBlock board, terminal
  at the top, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIG    -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Terminal: left screw to the battery's minus, right screw to its plus.

  TK01 XL LED, counted from its own square pad:
    GND    -> GND
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32, GPIO 5 on an
              ESP32-S3, GP15 on a Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIG is wired to. Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SIG_PIN = A0;
// The pin the TK01 is on. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 15.
const int LED_PIN = 9;

// Uno: 5.0 and 1023. Pico: 3.3 and 1023. The ESP32s do not use these.
const float VREF = 5.0;
const float ADC_MAX = 1023.0;

const float SCALE = 5.0;        // the terminal is 5 x SIG
const float CAL = 1.000;        // multimeter / sketch
const int SAMPLES = 16;

const float LOW_V = 7.5;        // warn below this
const float HYSTERESIS = 0.2;   // and stop only above LOW_V + this

bool warning = false;

float sigVolts() {
  float sum = 0;
  for (int i = 0; i < SAMPLES; i++) {
#if defined(ARDUINO_ARCH_ESP32)
    sum += analogReadMilliVolts(SIG_PIN) / 1000.0;
#else
    sum += analogRead(SIG_PIN) * VREF / ADC_MAX;
#endif
  }
  return sum / SAMPLES;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  float battery = sigVolts() * SCALE * CAL;

  if (!warning && battery < LOW_V) {
    warning = true;
  } else if (warning && battery > LOW_V + HYSTERESIS) {
    warning = false;   // only a real recovery, such as a new battery
  }
  digitalWrite(LED_PIN, warning ? HIGH : LOW);

  Serial.print("battery ");
  Serial.print(battery, 2);
  Serial.println(warning ? " V  LOW" : " V");
  delay(1000);
}
