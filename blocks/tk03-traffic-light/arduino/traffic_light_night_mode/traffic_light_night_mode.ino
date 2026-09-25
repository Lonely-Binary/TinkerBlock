/*
  Traffic Light - night mode, from the serial monitor   TK03 / /p/tk03

  Wiring. Count from the square pad on the TinkerBlock board, LEDs up,
  header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    RED    -> D9 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP13 on a Raspberry Pi Pico
    YELLOW -> D10 on an Uno, GPIO 26 on an ESP32, GPIO 5 on an ESP32-S3,
              GP14 on a Pico
    GREEN  -> D11 on an Uno, GPIO 27 on an ESP32, GPIO 6 on an ESP32-S3,
              GP15 on a Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Serial Monitor        115200 baud. Type n or d and Send.
    No library needed.
*/

// GPIO numbers. Uno: 9, 10, 11. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6.
// Pico: 13, 14, 15.
const int RED_PIN = 4;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;

struct Phase {
  bool red, yellow, green;
  unsigned long ms;
  const char *name;
};

const Phase PHASES[] = {
  // RED   YELLOW GREEN  ms
  { true,  false, false, 5000, "red" },
  { false, false, true,  5000, "green" },
  { false, true,  false, 2000, "yellow" },
};
const int PHASE_COUNT = sizeof(PHASES) / sizeof(PHASES[0]);
const unsigned long FLASH_MS = 500;  // night: 500 ms on, 500 ms off

int phase = 0;
unsigned long phaseStart = 0;
bool night = false;                  // the second piece of state
unsigned long flashStart = 0;

void lights(bool red, bool yellow, bool green) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
}

void show(int p) {
  lights(PHASES[p].red, PHASES[p].yellow, PHASES[p].green);
  Serial.println(PHASES[p].name);
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  show(phase);
  phaseStart = millis();
}

void loop() {
  unsigned long now = millis();

  // A command, if one has arrived. Never waits for one.
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'n' && !night) {
      night = true;
      flashStart = now;
      Serial.println("night: flashing yellow");
    } else if (c == 'd' && night) {
      night = false;
      phase = 0;                     // day always restarts on red
      phaseStart = now;
      show(phase);
    }
  }

  if (night) {
    // Yellow on for the first FLASH_MS of every two, off for the second.
    bool on = ((now - flashStart) / FLASH_MS) % 2 == 0;
    lights(false, on, false);
  } else if (now - phaseStart >= PHASES[phase].ms) {
    phase = (phase + 1) % PHASE_COUNT;
    phaseStart = now;
    show(phase);
  }
}
