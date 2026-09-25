"""
  Analog Microphone - first reading, MicroPython       TK27 / /p/tk27

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
    View > Plotter                to see the wave
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
MIC_PIN = 4

adc = ADC(Pin(MIC_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V

while True:
    print(adc.read_u16())             # one instant of the wave
    time.sleep_ms(2)
