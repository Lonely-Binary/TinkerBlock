"""
  Dual Axis Joystick - first read, MicroPython        TK23 / /p/tk23

  Wiring. Parts up, header along the bottom. Count from the square pad,
  which is GND at the left-hand end, rightwards:

    GND -> GND
    VCC -> 3V3   (never 5V: X, Y and SW all reach VCC)
    NC  -> nothing   (in no net on the board)
    X   -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
           GP26 on a Raspberry Pi Pico           (analog)
    Y   -> GPIO 35, GPIO 5, GP27      (same order, analog)
    SW  -> GPIO 25, GPIO 7, GP15      (digital)

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO numbers X, Y and SW are wired to.
# ESP32: 34 35 25. ESP32-S3: 4 5 7. Pico: 26 27 15.
X_PIN, Y_PIN, SW_PIN = 4, 5, 7

ESP = sys.platform == "esp32"          # ESP32 and ESP32-S3


def analog(pin):
    adc = ADC(Pin(pin))
    if ESP:
        adc.atten(ADC.ATTN_11DB)       # the full range, to about 3.1 V
    return adc


x_adc = analog(X_PIN)
y_adc = analog(Y_PIN)
sw = Pin(SW_PIN, Pin.IN)               # R10 on the board pulls it down

while True:
    print("X %5d  Y %5d  SW %d" % (
        x_adc.read_u16(), y_adc.read_u16(), sw.value()))
    time.sleep_ms(200)                 # five lines a second
