"""
  Ambient Light Sensor - first reading, MicroPython   TK20 / /p/tk20

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

adc = ADC(Pin(LIGHT_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V

while True:
    print("count", adc.read_u16())    # more light, higher
    time.sleep_ms(500)
