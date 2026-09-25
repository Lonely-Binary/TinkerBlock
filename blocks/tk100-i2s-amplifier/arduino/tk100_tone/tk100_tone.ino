// TK100 I2S Amplifier: first sound. A scale of seven notes on the speaker.
//
// Wiring, TK100 to ESP32-S3:
//   GND   -> GND
//   3V3   -> 3V3
//   LRCLK -> GPIO 16
//   BCLK  -> GPIO 15
//   DIN   -> GPIO 17
//   CTRL  -> GPIO 18   (HIGH plays, LOW or unconnected is off)
//   Speaker, 4 or 8 ohm, in the white two-pin socket.
//
// Arduino IDE: Tools > Board > "ESP32S3 Dev Module" (esp32 core 3.x),
// USB CDC On Boot: Enabled. Serial Monitor at 115200.

#include <ESP_I2S.h>

const int PIN_BCLK   = 15;
const int PIN_LRCLK  = 16;
const int PIN_DIN    = 17;
const int PIN_AMP_ON = 18;

const int RATE = 16000;                 // samples per second
I2SClass i2s;

// One frame is a left sample then a right sample. The amplifier plays
// only one of them, so the same value goes in both.
void playNote(float hz, int ms) {
  const int16_t level = 6000;           // about a fifth of full scale
  int16_t frame[2];
  float phase = 0;
  float step = 2 * PI * hz / RATE;
  for (int i = 0; i < RATE * ms / 1000; i++) {
    frame[0] = frame[1] = (int16_t)(level * sinf(phase));
    i2s.write((uint8_t *)frame, sizeof(frame));
    phase += step;
    if (phase > 2 * PI) phase -= 2 * PI;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_AMP_ON, OUTPUT);
  digitalWrite(PIN_AMP_ON, HIGH);       // wake the amplifier

  i2s.setPins(PIN_BCLK, PIN_LRCLK, PIN_DIN);
  if (!i2s.begin(I2S_MODE_STD, RATE, I2S_DATA_BIT_WIDTH_16BIT,
                 I2S_SLOT_MODE_STEREO)) {
    Serial.println("I2S did not start");
    while (true) delay(1000);
  }
  Serial.println("Playing");
}

void loop() {
  const float notes[] = {261.63, 293.66, 329.63, 349.23,
                         392.00, 440.00, 493.88};
  for (float hz : notes) {
    Serial.println(hz);
    playNote(hz, 400);
    playNote(0, 100);                   // a short gap
  }
  delay(1000);
}
