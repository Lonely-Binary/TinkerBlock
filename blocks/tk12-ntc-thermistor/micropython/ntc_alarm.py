"""
  NTC Thermistor - a temperature alarm, MicroPython   TK12 / /p/tk12

  Wiring, the TK12. Count from the square pad, parts up, header at
  the bottom:

    GND    -> GND
    VCC    -> 3V3   (SIGNAL is a fraction of VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, math, sys and time are built in.
"""

import math
import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
SENSOR_PIN = 4
# The TK01's SIGNAL. ESP32: 4. ESP32-S3: 5. Pico: 15.
LED_PIN = 5

ALARM_C = 28.0      # on at or above this
HYST_C = 1.0        # off only below ALARM_C - HYST_C

SAMPLES = 32
R_FIXED = 10000     # the 10 kOhm from VCC to SIGNAL
R25 = 10000         # the thermistor at 25 C: the "103"
B = 3950            # its B value: the "3950"
T0 = 298.15         # 25 C in kelvin
VCC = 3.3           # ESP32 only: your 3V3 pin, measured

adc = ADC(Pin(SENSOR_PIN))
ESP = sys.platform == "esp32"         # ESP32 and ESP32-S3
if ESP:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V
led = Pin(LED_PIN, Pin.OUT)


def thermistor_ohms():
    total = 0
    for _ in range(SAMPLES):
        if ESP:
            total += adc.read_uv() / 1000000 / VCC
        else:
            total += adc.read_u16() / 65535
    ratio = total / SAMPLES
    if ratio <= 0 or ratio >= 1:
        return None
    return R_FIXED * ratio / (1 - ratio)


def celsius(ohms):
    return 1 / (1 / T0 + math.log(ohms / R25) / B) - 273.15


alarm_on = False
while True:
    r = thermistor_ohms()
    if r is None:
        print("SIGNAL at 0: check VCC. At the top: check GND.")
        time.sleep_ms(500)
        continue
    c = celsius(r)

    if not alarm_on and c >= ALARM_C:
        alarm_on = True
    if alarm_on and c < ALARM_C - HYST_C:
        alarm_on = False
    led.value(1 if alarm_on else 0)

    print("%.2f C%s" % (c, "  ALARM" if alarm_on else ""))
    time.sleep_ms(250)
