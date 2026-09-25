"""
  Reflective Optical Sensor - first reading, MicroPython  TK57 / /p/tk57

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: SIGNAL rises to VCC over a dark
                     surface, and these pins take 3.3 V)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 34 on an ESP32,
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
SENSOR_PIN = 4

adc = ADC(Pin(SENSOR_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V

while True:
    level = adc.read_u16()        # 0 to 65535; more light back, lower
    print(level, "#" * (level * 40 // 65535))
    time.sleep_ms(100)
