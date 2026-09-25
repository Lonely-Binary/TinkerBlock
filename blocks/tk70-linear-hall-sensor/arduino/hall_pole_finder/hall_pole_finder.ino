/*
  Linear Hall Effect Sensor - pole finder               TK70 / /p/tk70

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno.
              Your board's logic voltage: a strong field takes
              SIGNAL up to about 0.8 x VCC.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Keep magnets away while it starts: it measures its zero then.
  Hold a magnet's face to the front of the board, over the chip.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int HALL_PIN = 4;

#if defined(ARDUINO_ARCH_AVR)
// Uno, VCC on 5V: 3.25 mV per gauss, typical; ADC full scale 5 V.
const float MV_PER_GAUSS = 3.25;
const float FULL_SCALE_MV = 5000.0;
#else
// VCC on 3V3: about 2.1 mV per gauss; Pico's ADC full scale 3.3 V.
const float MV_PER_GAUSS = 2.1;
const float FULL_SCALE_MV = 3300.0;
#endif

// The data sheet's SOT-23 drawing: north on the front raises it.
const bool NORTH_RAISES = true;

// Name a pole once the field passes ENTER_G; let it go only
// when it falls under LEAVE_G, so the edge does not flicker.
const float ENTER_G = 20;
const float LEAVE_G = 10;

const char* NAMES[] = {"no magnet", "north pole", "south pole"};
int state = 0;   // 0 none, 1 north, 2 south
float zeroMv;

float readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(HALL_PIN);   // calibrated in the chip
#else
  return analogRead(HALL_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

float averageMv(int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) sum += readMilliVolts();
  return sum / n;
}

int poleOf(float gauss) {
  return (gauss > 0) == NORTH_RAISES ? 1 : 2;
}

// One # for every 20 G: the bar grows as the magnet comes closer.
void printBar(float strength) {
  int n = strength / 20;
  if (n > 23) n = 23;
  Serial.print("  ");
  for (int i = 0; i < n; i++) Serial.print('#');
  Serial.print(' ');
  Serial.print(strength, 0);
  Serial.println(" G");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  zeroMv = averageMv(64);   // no magnet near, please
  Serial.println(NAMES[state]);
}

void loop() {
  float gauss = (averageMv(16) - zeroMv) / MV_PER_GAUSS;
  float strength = fabs(gauss);
  int next = state;

  if (strength >= ENTER_G) {
    next = poleOf(gauss);
  } else if (strength < LEAVE_G) {
    next = 0;
  }

  if (next != state) {
    state = next;
    Serial.println(NAMES[state]);
  }
  if (state != 0) printBar(strength);
  delay(200);
}
