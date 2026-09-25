// TK103 PCM5102 DAC: a tone in the left ear, then the right, then both.
//
// Wiring, TK103 to ESP32-S3:
//   GND  -> GND
//   3V3  -> 3V3
//   BCK  -> GPIO 42
//   DIN  -> GPIO 41
//   LRCK -> GPIO 40
//   XSMT -> GPIO 39   (optional: HIGH plays, LOW mutes; pulled HIGH on
//                      the board when nothing is connected)
//   Headphones or powered speakers in the 3.5 mm jack. Never a bare
//   speaker, never a speaker amplifier's output.
//
// Arduino IDE: Tools > Board > "ESP32S3 Dev Module" (esp32 core 3.x),
// USB CDC On Boot: Enabled. Serial Monitor at 115200.

#include <ESP_I2S.h>

const int PIN_BCK  = 42;
const int PIN_DIN  = 41;
const int PIN_LRCK = 40;
const int PIN_XSMT = 39;

const int RATE = 44100;
I2SClass dac;

// left and right are 0 or 1: which ear hears the tone.
void tone2(float hz, int ms, int left, int right) {
  const int16_t level = 3000;           // gentle in headphones
  int16_t frame[2];
  float phase = 0, step = 2 * PI * hz / RATE;
  for (long i = 0; i < (long)RATE * ms / 1000; i++) {
    int16_t s = (int16_t)(level * sinf(phase));
    frame[0] = left ? s : 0;            // the left sample goes first
    frame[1] = right ? s : 0;
    dac.write((uint8_t *)frame, sizeof(frame));
    phase += step;
    if (phase > 2 * PI) phase -= 2 * PI;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_XSMT, OUTPUT);
  digitalWrite(PIN_XSMT, HIGH);         // un-mute

  dac.setPins(PIN_BCK, PIN_LRCK, PIN_DIN);
  if (!dac.begin(I2S_MODE_STD, RATE, I2S_DATA_BIT_WIDTH_16BIT,
                 I2S_SLOT_MODE_STEREO)) {
    Serial.println("I2S did not start");
    while (true) delay(1000);
  }
}

void loop() {
  Serial.println("left");
  tone2(440, 1000, 1, 0);
  Serial.println("right");
  tone2(440, 1000, 0, 1);
  Serial.println("both");
  tone2(440, 1000, 1, 1);
  tone2(0, 1000, 0, 0);
}
