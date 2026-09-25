// Lesson 27: NTC thermistor read temperature (°C)
#include <math.h>   // for log() to compute natural logarithm

#define TEMP_PIN A0  // TK12 SIGNAL to A0

// NTC parameters (typical for 10K thermistor; adjust BETA or R_SERIES if inaccurate)
const float VCC = 5.0;           // supply 5V
const float R0 = 10000.0;        // NTC resistance at 25°C (Ω)
const float T0 = 25.0 + 273.15;  // reference temperature (Kelvin)
const float BETA = 3950.0;       // Beta (datasheet often 3950; try 3435 or 4100)
const float R_SERIES = 10000.0;  // series resistor in divider (Ω)

void setup() {
  Serial.begin(9600);
  Serial.println("NTC temperature sensor started");
}

void loop() {
  int raw = analogRead(TEMP_PIN);   // read A0, 0–1023
  float voltage = raw * (VCC / 1023.0);  // convert to voltage 0–5V

  // Voltage divider: solve for NTC resistance: R_NTC = R_SERIES * Vout / (VCC - Vout)
  float rNTC = R0;
  if (voltage > 0 && voltage < VCC) {
    rNTC = R_SERIES * voltage / (VCC - voltage);
  }

  // Beta equation for temperature (Kelvin): T = 1 / (1/T0 + (1/BETA)*ln(R/R0))
  float tempK = T0;
  if (rNTC > 0 && rNTC < 1000000.0) {
    float lnR = log(rNTC / R0);
    tempK = 1.0 / (1.0 / T0 + (1.0 / BETA) * lnR);
  }
  float tempC = tempK - 273.15;  // convert to Celsius
  if (tempC < -50.0) tempC = -50.0;   // clamp display range, avoid outliers
  if (tempC > 150.0) tempC = 150.0;

  Serial.print("Temperature: ");
  Serial.print(tempC, 2);
  Serial.println(" °C");

  delay(1000);   // update once per second to avoid flooding serial
}
