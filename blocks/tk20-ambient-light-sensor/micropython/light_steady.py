"""
  Ambient Light Sensor - a steady reading, MicroPython  TK20 / /p/tk20

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: bright light takes SIGNAL towards VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
LIGHT_PIN = 4

WINDOW_MS = 50      # 5 cycles of a 100 Hz ripple, 6 of a 120 Hz one

adc = ADC(Pin(LIGHT_PIN))
ESP = sys.platform == "esp32"         # ESP32 and ESP32-S3
if ESP:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V


def read_millivolts():
    if ESP:
        return adc.read_uv() / 1000   # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


def read_steady_millivolts():
    total = 0
    n = 0
    start = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), start) < WINDOW_MS:
        total += read_millivolts()
        n += 1
    return total / n


while True:
    one = read_millivolts()
    mean = read_steady_millivolts()
    print("one read %.0f mV    50 ms mean %.0f mV" % (one, mean))
    time.sleep_ms(250)
