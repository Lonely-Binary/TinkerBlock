/*
  DHT22 - a comfort reading                             TK39 / /p/tk39

  Wiring. Count from the square pad on the TinkerBlock board, sensor
  side up, header at the bottom:

    GND  -> GND
    VCC  -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC   -> nothing   (unconnected on the board)
    DATA -> D2 on an Uno, GPIO 18 on an ESP32,
            GPIO 4 on an ESP32-S3, GP2 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Manage Libraries      DHT sensor library, by Adafruit.
                                  Say yes to Adafruit Unified Sensor.

  Mount the sensor in free air, off the board: a warm sensor reads
  dry, and the dew point inherits the error.
*/

#include <DHT.h>
#include <math.h>

// Uno: 2. ESP32: 18. ESP32-S3: 4. Pico: 2.
#define DHT_PIN  4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// Magnus-Tetens, Sonntag 1990 coefficients: about 0.35 C
// between -45 and 60 C.
const float MAGNUS_A = 17.62;
const float MAGNUS_B = 243.12;

float dewPoint(float tC, float rh) {
  if (rh < 1) rh = 1;       // log(0) is not a temperature
  float g = log(rh / 100.0) + (MAGNUS_A * tC) / (MAGNUS_B + tC);
  return (MAGNUS_B * g) / (MAGNUS_A - g);
}

const char *howItFeels(float dp) {
  if (dp < 10) return "dry";
  if (dp < 16) return "comfortable";
  if (dp < 18) return "sticky";
  if (dp < 21) return "humid";
  return "oppressive";
}

const unsigned long EVERY_MS = 10000;
unsigned long last = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  last = millis();
}

void loop() {
  if (millis() - last < EVERY_MS) return;
  last += EVERY_MS;

  float rh = dht.readHumidity();
  float t  = dht.readTemperature();
  if (isnan(rh) || isnan(t)) {
    Serial.println("read failed");
    return;
  }

  float dp = dewPoint(t, rh);
  Serial.print(t, 1);  Serial.print(" C  ");
  Serial.print(rh, 1); Serial.print(" %  dew point ");
  Serial.print(dp, 1); Serial.print(" C  ");
  Serial.println(howItFeels(dp));
}
