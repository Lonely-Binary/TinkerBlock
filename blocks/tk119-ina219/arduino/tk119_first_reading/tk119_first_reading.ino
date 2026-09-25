// TK119 INA219: the first reading, on an ESP32-S3.
//
// Wiring, TK119 header left to right (parts up, pins down):
//   GND -> ESP32 GND
//   3V3 -> ESP32 3V3
//   SCL -> GPIO9     (the third pin: the clock comes first)
//   SDA -> GPIO8
// Then the rail, last: supply + to POWER +, supply - to
// POWER -, LOAD + to the load's +, LOAD - to the load's -.
//
// Arduino IDE: Tools > Board > esp32 > ESP32S3 Dev Module,
// Tools > USB CDC On Boot > Enabled, then Tools > Port.
// Library Manager: install "Adafruit INA219" (it brings
// Adafruit BusIO). Serial Monitor at 115200.

#include <Wire.h>
#include <Adafruit_INA219.h>

const int SDA_PIN = 8, SCL_PIN = 9;

// 0x40 with both pads open; 0x41 A0, 0x44 A1, 0x45 both.
Adafruit_INA219 ina(0x40);

// The current with nothing in LOAD, in mA: the chip's zero
// error. Read it once and put it here.
const float ZERO_MA = 0.0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!ina.begin(&Wire)) {
    Serial.println("No INA219 at 0x40: is SCL the third pin?");
    while (true) delay(1000);
  }
  // Gain /8: up to 3.2 A, 0.1 mA a count. The default.
  ina.setCalibration_32V_2A();
  Serial.println("shunt_mV  bus_V  mA  mW  supply_V");
}

void loop() {
  float shunt = ina.getShuntVoltage_mV();   // measured
  float bus = ina.getBusVoltage_V();        // measured, LOAD +
  float ma = ina.getCurrent_mA() - ZERO_MA; // worked out
  float mw = ina.getPower_mW();             // worked out
  float supply = bus + shunt / 1000;        // at POWER +

  Serial.printf("%.2f  %.3f  %.1f  %.0f  %.3f\n",
                shunt, bus, ma, mw, supply);
  delay(500);
}
