"""
  Analog to Digital Signal - a light switch           TK29 / /p/tk29

  A TK20 Ambient Light Sensor pushed into IN, parts facing the same
  way. Slide switch at L: DIG is HIGH while it is darker than the knob.

  Wiring, the TK29's OUT. Count from the square pad, parts up, OUT at
  the bottom:

    GND -> GND
    VCC -> 3V3   (never 5V: DIG is pulled up to VCC)
    DIG -> GPIO 25 on an ESP32, GPIO 7 on an ESP32-S3,
           GP15 on a Raspberry Pi Pico
    ANA -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
           GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way. Point it away from the TK20:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3,
              GP14 on a Raspberry Pi Pico

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
# The TK01's SIGNAL. ESP32: 4. ESP32-S3: 5. Pico: 14.
LAMP_PIN = 5

dig = Pin(DIG_PIN, Pin.IN)            # no pull: the block has its own
lamp = Pin(LAMP_PIN, Pin.OUT)
adc = ADC(Pin(ANA_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V

while True:
    dark = dig.value()                # at L: 1 when darker
    lamp.value(dark)                  # the lamp copies DIG
    print("dark " if dark else "light", " ANA", adc.read_u16())
    time.sleep_ms(100)
