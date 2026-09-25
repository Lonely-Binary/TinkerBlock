// TK101 + TK103: hear the microphone live in headphones.
//
// Wiring, TK101 microphone to ESP32-S3:
//   GND -> GND, 3V3 -> 3V3, CLK -> GPIO 5, DATA -> GPIO 6
// Wiring, TK103 DAC to ESP32-S3:
//   GND -> GND, 3V3 -> 3V3
//   BCK -> GPIO 42, DIN -> GPIO 41, LRCK -> GPIO 40, XSMT -> GPIO 39
//   Headphones in the jack. Headphones, not a speaker: a speaker near
//   the microphone feeds back into a howl.
//
// Arduino IDE: Tools > Board > "ESP32S3 Dev Module" (esp32 core 3.x),
// USB CDC On Boot: Enabled. Serial Monitor at 115200.

#include <ESP_I2S.h>

const int PIN_CLK = 5, PIN_DATA = 6;
const int PIN_BCK = 42, PIN_DIN = 41, PIN_LRCK = 40, PIN_XSMT = 39;
const int RATE = 16000;
const int GAIN = 4;                     // start low in headphones

I2SClass mic;                           // PDM in, on I2S0
I2SClass dac;                           // standard out, on I2S1

int16_t in[256];                        // 16 ms of sound
int32_t out[512];                       // the same, as left-right pairs
float offset = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_XSMT, OUTPUT);
  digitalWrite(PIN_XSMT, LOW);          // muted while the clocks start

  mic.setPinsPdmRx(PIN_CLK, PIN_DATA);
  dac.setPins(PIN_BCK, PIN_LRCK, PIN_DIN);

  // The DAC gets 32-bit slots: at 16 kHz the PCM5102 locks to a bit
  // clock of 64 per sample, and 16-bit stereo would give it only 32.
  bool ok =
    mic.begin(I2S_MODE_PDM_RX, RATE, I2S_DATA_BIT_WIDTH_16BIT,
              I2S_SLOT_MODE_MONO) &&
    dac.begin(I2S_MODE_STD, RATE, I2S_DATA_BIT_WIDTH_32BIT,
              I2S_SLOT_MODE_STEREO);
  if (!ok) {
    Serial.println("I2S did not start");
    while (true) delay(1000);
  }
  delay(50);
  digitalWrite(PIN_XSMT, HIGH);         // un-mute: the DAC ramps up
  Serial.println("Listening");
}

void loop() {
  size_t n = mic.readBytes((char *)in, sizeof(in)) / 2;
  int peak = 0;
  for (size_t i = 0; i < n; i++) {
    offset += (in[i] - offset) * 0.001f;          // slow average
    long s = (long)(in[i] - offset) * GAIN;
    s = constrain(s, -32768, 32767);
    peak = max(peak, (int)abs(s));
    out[2 * i] = out[2 * i + 1] = s * 65536;      // top 16 of 32 bits
  }
  dac.write((uint8_t *)out, n * 2 * sizeof(int32_t));

  static uint32_t last = 0;
  if (millis() - last > 500) {
    last = millis();
    Serial.printf("peak %d\n", peak);
  }
}
