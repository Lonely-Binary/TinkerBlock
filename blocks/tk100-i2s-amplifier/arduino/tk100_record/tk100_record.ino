// TK100 + TK101: press BOOT, speak for three seconds, hear it back.
//
// Wiring, TK100 amplifier to ESP32-S3:
//   GND -> GND, 3V3 -> 3V3
//   BCLK -> GPIO 15, LRCLK -> GPIO 16, DIN -> GPIO 17, CTRL -> GPIO 18
//   Speaker in the white socket.
// Wiring, TK101 microphone to ESP32-S3:
//   GND -> GND, 3V3 -> 3V3, CLK -> GPIO 5, DATA -> GPIO 6
// The BOOT button is GPIO 0 on the board itself.
//
// Arduino IDE: Tools > Board > "ESP32S3 Dev Module" (esp32 core 3.x),
// USB CDC On Boot: Enabled. Serial Monitor at 115200.

#include <ESP_I2S.h>

const int PIN_BCLK = 15, PIN_LRCLK = 16, PIN_DIN = 17, PIN_AMP_ON = 18;
const int PIN_CLK = 5, PIN_DATA = 6;
const int PIN_BUTTON = 0;               // BOOT, LOW while pressed

const int RATE = 16000;
const int SECONDS = 3;
const int GAIN = 8;                     // speech is quiet; raise if faint

I2SClass mic;                           // PDM in
I2SClass amp;                           // standard I2S out
int16_t *clip;                          // 3 s x 16000 = 48000 samples
const size_t CLIP_LEN = RATE * SECONDS;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_AMP_ON, OUTPUT);
  digitalWrite(PIN_AMP_ON, LOW);        // amplifier off until playback

  clip = (int16_t *)malloc(CLIP_LEN * sizeof(int16_t));   // 96 kB

  mic.setPinsPdmRx(PIN_CLK, PIN_DATA);
  amp.setPins(PIN_BCLK, PIN_LRCLK, PIN_DIN);
  bool ok = clip &&
    mic.begin(I2S_MODE_PDM_RX, RATE, I2S_DATA_BIT_WIDTH_16BIT,
              I2S_SLOT_MODE_MONO) &&
    amp.begin(I2S_MODE_STD, RATE, I2S_DATA_BIT_WIDTH_16BIT,
              I2S_SLOT_MODE_STEREO);
  if (!ok) {
    Serial.println("Start failed: check memory and pins");
    while (true) delay(1000);
  }
  Serial.println("Press BOOT to record");
}

void record() {
  Serial.println("Recording...");
  mic.readBytes((char *)clip, 1024);    // drop the stale start
  mic.readBytes((char *)clip, CLIP_LEN * sizeof(int16_t));

  long sum = 0;                         // remove the offset
  for (size_t i = 0; i < CLIP_LEN; i++) sum += clip[i];
  int offset = sum / (long)CLIP_LEN;
  for (size_t i = 0; i < CLIP_LEN; i++) {
    long s = (long)(clip[i] - offset) * GAIN;
    clip[i] = constrain(s, -32768, 32767);
  }
}

void play() {
  Serial.println("Playing...");
  digitalWrite(PIN_AMP_ON, HIGH);
  int16_t frame[2];
  for (size_t i = 0; i < CLIP_LEN; i++) {
    frame[0] = frame[1] = clip[i];      // same sample, both channels
    amp.write((uint8_t *)frame, sizeof(frame));
  }
  delay(100);                           // let the last buffer drain
  digitalWrite(PIN_AMP_ON, LOW);
  Serial.println("Press BOOT to record");
}

void loop() {
  if (digitalRead(PIN_BUTTON) == LOW) {
    record();
    play();
  }
}
