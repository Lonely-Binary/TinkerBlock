/*
  Reflective Optical Sensor - a counter                 TK57 / /p/tk57

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3, on every board, the Uno included.
              SIGNAL rises to VCC over a dark surface, and 5V
              overloads the block's 62-ohm emitter resistor.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 34 on an ESP32,
              A0 on an Uno, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.

  Mount the block face down, 2 to 5 mm over the surface. It learns
  two readings at start: the background, then the mark to count.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SENSOR_PIN = 4;

// Two lines, not one: a mark counts when the reading passes
// ENTER of the way from background to mark, and the counter
// re-arms only when it falls back past LEAVE.
const float ENTER = 0.6;
const float LEAVE = 0.4;

int background;
int mark;
bool onMark = false;
unsigned long count = 0;

int readAverage() {
  long sum = 0;
  for (int i = 0; i < 8; i++) sum += analogRead(SENSOR_PIN);
  return sum / 8;
}

int learn(const char *what) {
  Serial.print("Hold it over ");
  Serial.print(what);
  Serial.println(" ...");
  delay(3000);
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += readAverage();
    delay(10);
  }
  return sum / 50;
}

// How far a reading has gone from background towards mark.
float progress(int level) {
  return float(level - background) / float(mark - background);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  background = learn("the background");
  mark = learn("the mark to count");
  Serial.print("background ");
  Serial.print(background);
  Serial.print(", mark ");
  Serial.println(mark);
  if (abs(mark - background) < 40) {
    Serial.println("Too alike to tell apart: press reset.");
  }
}

void loop() {
  float p = progress(readAverage());

  if (!onMark && p >= ENTER) {
    onMark = true;
    count++;
    Serial.print("count ");
    Serial.println(count);
  } else if (onMark && p <= LEAVE) {
    onMark = false;   // back on the background: re-armed
  }
}
