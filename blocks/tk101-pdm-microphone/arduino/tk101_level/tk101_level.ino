// TK101 PDM Microphone: how loud is the room? Open Tools > Serial Plotter.
//
// Wiring, TK101 to ESP32-S3:
//   GND  -> GND
//   3V3  -> 3V3
//   CLK  -> GPIO 5
//   DATA -> GPIO 6
//
// Arduino IDE: Tools > Board > "ESP32S3 Dev Module" (esp32 core 3.x),
// USB CDC On Boot: Enabled. Serial Plotter at 115200.

#include <ESP_I2S.h>

const int PIN_CLK  = 5;
const int PIN_DATA = 6;
const int RATE = 16000;

I2SClass mic;
int16_t samples[512];                   // 32 ms at 16 kHz

void setup() {
  Serial.begin(115200);
  mic.setPinsPdmRx(PIN_CLK, PIN_DATA);
  if (!mic.begin(I2S_MODE_PDM_RX, RATE, I2S_DATA_BIT_WIDTH_16BIT,
                 I2S_SLOT_MODE_MONO)) {
    Serial.println("PDM did not start");
    while (true) delay(1000);
  }
}

void loop() {
  size_t n = mic.readBytes((char *)samples, sizeof(samples)) / 2;
  if (n == 0) return;

  // Take out the average first: a microphone's samples sit on a small
  // offset, and it would read as sound that is not there.
  long sum = 0;
  for (size_t i = 0; i < n; i++) sum += samples[i];
  int offset = sum / (long)n;

  int peak = 0;
  for (size_t i = 0; i < n; i++) {
    int a = abs(samples[i] - offset);
    if (a > peak) peak = a;
  }
  Serial.print("peak:");
  Serial.println(peak);                 // 0 to 32767
}
