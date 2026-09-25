"""
  Analog to Digital Signal - first read, MicroPython  TK29 / /p/tk29

  A TK08 Rotary Potentiometer, or any analog block, pushed into IN,
  parts facing the same way. Slide switch at H.

  Wiring, OUT to your board. Count from the square pad, parts up,
  OUT at the bottom:

    GND -> GND
    VCC -> 3V3   (never 5V: DIG is pulled up to VCC)
    DIG -> GPIO 25 on an ESP32, GPIO 7 on an ESP32-S3,
           GP15 on a Raspberry Pi Pico
    ANA -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
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

# DIG_PIN, then ANA_PIN.
# ESP32: 25 and 34. ESP32-S3: 7 and 4. Pico: 15 and 26.
DIG_PIN = 7
ANA_PIN = 4

dig = Pin(DIG_PIN, Pin.IN)            # no pull: the block has its own
adc = ADC(Pin(ANA_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V

while True:
    print("DIG", dig.value(), "  ANA", adc.read_u16())
    time.sleep_ms(250)
