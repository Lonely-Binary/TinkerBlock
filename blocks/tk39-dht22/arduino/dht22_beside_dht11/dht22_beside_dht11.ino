/*
  DHT22 beside DHT11                           TK39 + TK38 / /p/dht

  Wiring. Both boards, counting from the square pad, sensor side up,
  header at the bottom:

    GND  -> GND
    VCC  -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC   -> nothing   (unconnected on the board)
    DATA -> DHT22 (white): D2 on an Uno, GPIO 18 on an ESP32,
                           GPIO 4 on an ESP32-S3, GP2 on a Pico
            DHT11 (blue):  D3 on an Uno, GPIO 19 on an ESP32,
                           GPIO 5 on an ESP32-S3, GP3 on a Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Manage Libraries      DHT sensor library, by Adafruit.
                                  Say yes to Adafruit Unified Sensor.

  Mount the two the same: same height, same air, same distance from
  the board. Otherwise you are measuring where they are.
*/

#include <DHT.h>

// Uno: 2 and 3. ESP32: 18 and 19. ESP32-S3: 4 and 5. Pico: 2 and 3.
#define PIN_22 4
#define PIN_11 5

// Nothing on the wire says which is which: the type word does.
DHT white(PIN_22, DHT22);
DHT blue(PIN_11, DHT11);

const unsigned long EVERY_MS = 5000;
unsigned long last = 0;

void setup() {
  Serial.begin(115200);
  white.begin();
  blue.begin();
  last = millis();
  Serial.println("t22,rh22,t11,rh11,dT,dRH");
}

void loop() {
  if (millis() - last < EVERY_MS) return;
  last += EVERY_MS;

  // About 7 ms for the DHT22, 25 ms for the DHT11: most of the
  // DHT11's is its 20 ms start pulse.
  float t22 = white.readTemperature(), h22 = white.readHumidity();
  float t11 = blue.readTemperature(),  h11 = blue.readHumidity();

  if (isnan(t22) || isnan(h22) || isnan(t11) || isnan(h11)) {
    Serial.println("read failed");
    return;
  }

  Serial.print(t22, 1);       Serial.print(",");
  Serial.print(h22, 1);       Serial.print(",");
  Serial.print(t11, 1);       Serial.print(",");
  Serial.print(h11, 1);       Serial.print(",");
  Serial.print(t11 - t22, 1); Serial.print(",");
  Serial.println(h11 - h22, 1);
}
