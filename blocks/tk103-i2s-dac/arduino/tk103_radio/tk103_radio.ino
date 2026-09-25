// TK103 PCM5102 DAC: internet radio in stereo, from an MP3 stream.
//
// Wiring, TK103 to ESP32-S3:
//   GND -> GND, 3V3 -> 3V3
//   BCK -> GPIO 42, DIN -> GPIO 41, LRCK -> GPIO 40, XSMT -> GPIO 39
//   Headphones or powered speakers in the jack.
//
// Libraries (Sketch > Include Library > Add .ZIP Library), from GitHub:
//   pschatzmann/arduino-audio-tools and pschatzmann/arduino-libhelix
// Arduino IDE: Tools > Board > "ESP32S3 Dev Module" (esp32 core 3.x),
// USB CDC On Boot: Enabled.
// Type a number from 0 to 1 in the Serial Monitor (115200) for volume.

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/Communication/AudioHttp.h"

const char *SSID = "your-network";
const char *PASSWORD = "your-password";
const char *STATION = "http://stream.srg-ssr.ch/m/rsj/mp3_128";

const int PIN_BCK = 42, PIN_DIN = 41, PIN_LRCK = 40, PIN_XSMT = 39;

URLStream url(SSID, PASSWORD);          // bytes from the station
I2SStream dac;                          // samples to the TK103
VolumeStream volume(dac);               // scales every sample
EncodedAudioStream mp3(&volume, new MP3DecoderHelix());
StreamCopy copier(mp3, url);            // moves one buffer per call

void setup() {
  Serial.begin(115200);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
  pinMode(PIN_XSMT, OUTPUT);
  digitalWrite(PIN_XSMT, HIGH);

  auto cfg = dac.defaultConfig(TX_MODE);
  cfg.pin_bck = PIN_BCK;
  cfg.pin_ws = PIN_LRCK;
  cfg.pin_data = PIN_DIN;
  cfg.sample_rate = 44100;              // the decoder corrects this
  cfg.channels = 2;
  cfg.bits_per_sample = 16;
  dac.begin(cfg);

  volume.begin(cfg);
  volume.setVolume(0.3);
  mp3.begin();                          // tells the DAC the real rate
  url.begin(STATION, "audio/mp3");
}

void loop() {
  copier.copy();

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();                        // drops the line ending
    float v = line.toFloat();
    if (line.length() > 0 && v >= 0 && v <= 1) {
      volume.setVolume(v);
      Serial.printf("volume %.2f\n", v);
    }
  }
}
