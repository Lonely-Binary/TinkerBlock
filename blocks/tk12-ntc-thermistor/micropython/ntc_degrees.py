"""
  NTC Thermistor - counts to degrees, MicroPython     TK12 / /p/tk12

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (SIGNAL is a fraction of VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

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

R_FIXED = 10000     # the 10 kOhm from VCC to SIGNAL
R25 = 10000         # the thermistor at 25 C: the "103"
B = 3950            # its B value: the "3950"
T0 = 298.15         # 25 C in kelvin
VCC = 3.3           # ESP32 only: your 3V3 pin, measured

adc = ADC(Pin(SENSOR_PIN))
ESP = sys.platform == "esp32"         # ESP32 and ESP32-S3
if ESP:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V


def thermistor_ohms():
    if ESP:
        ratio = adc.read_uv() / 1000000 / VCC   # own reference
    else:
        ratio = adc.read_u16() / 65535          # already a fraction
    if ratio <= 0 or ratio >= 1:
        return None                             # SIGNAL at either end
    return R_FIXED * ratio / (1 - ratio)


def celsius(ohms):
    return 1 / (1 / T0 + math.log(ohms / R25) / B) - 273.15


while True:
    r = thermistor_ohms()
    if r is None:
        print("SIGNAL at 0: check VCC. At the top: check GND.")
    else:
        print("%.0f ohm   %.1f C" % (r, celsius(r)))
    time.sleep(1)
