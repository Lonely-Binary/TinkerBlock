"""
  Analog Microphone - above the quiet swing, MicroPython
                                                      TK27 / /p/tk27

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: a loud sound takes SIGNAL up to VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
MIC_PIN = 4

WINDOW_MS = 50
CALIBRATE_MS = 1000

adc = ADC(Pin(MIC_PIN))
ESP = sys.platform == "esp32"         # ESP32 and ESP32-S3
if ESP:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V


def read_millivolts():
    if ESP:
        return adc.read_uv() / 1000   # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


def swing_millivolts():
    lo = 100000
    hi = -1
    start = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), start) < WINDOW_MS:
        mv = read_millivolts()
        lo = min(lo, mv)
        hi = max(hi, mv)
    return hi - lo


print("Measuring the quiet swing. Keep quiet...")
quiet = 0
start = time.ticks_ms()
while time.ticks_diff(time.ticks_ms(), start) < CALIBRATE_MS:
    quiet = max(quiet, swing_millivolts())
print("Quiet swing %.0f mV" % quiet)

while True:
    swing = swing_millivolts()
    above = max(0, swing - quiet)     # what the room adds
    print("swing %.0f mV, above quiet %.0f" % (swing, above))
