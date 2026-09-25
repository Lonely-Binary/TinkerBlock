"""
  Slim Joystick - a centre and a dead zone, MicroPython   TK21 / /p/tk21

  Wiring. Parts up, header along the top. Count from the square pad,
  which is GND at the right-hand end, leftwards:

    GND  -> GND
    3V3  -> 3V3   (the header is printed 3V3; never 5V)
    BTNS -> GPIO 32 on an ESP32, GPIO 6 on an ESP32-S3,
            GP28 on a Raspberry Pi Pico          (analog)
    X    -> GPIO 34, GPIO 4, GP26      (same order, analog)
    Y    -> GPIO 35, GPIO 5, GP27      (analog)
    KEY  -> GPIO 25, GPIO 7, GP15      (digital)

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO numbers X, Y, BTNS and KEY are wired to.
# ESP32: 34 35 32 25. ESP32-S3: 4 5 6 7. Pico: 26 27 28 15.
X_PIN, Y_PIN, BTNS_PIN, KEY_PIN = 4, 5, 6, 7

DEAD_ZONE = 10              # per cent either side of the centre
EDGE_GUESS_MV = 1000        # the ends, until the stick shows them

ESP = sys.platform == "esp32"          # ESP32 and ESP32-S3


def analog(pin):
    adc = ADC(Pin(pin))
    if ESP:
        adc.atten(ADC.ATTN_11DB)       # the full range, to about 3.1 V
    return adc


def millivolts(adc):
    if ESP:
        return adc.read_uv() / 1000    # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


def average_mv(adc):
    return sum(millivolts(adc) for _ in range(16)) / 16


class Axis:
    def __init__(self, pin):
        self.adc = analog(pin)
        self.centre = average_mv(self.adc)    # hands off the stick
        self.lo = self.centre - EDGE_GUESS_MV
        self.hi = self.centre + EDGE_GUESS_MV

    def read(self):
        mv = average_mv(self.adc)
        self.lo = min(self.lo, mv)
        self.hi = max(self.hi, mv)
        if mv >= self.centre:
            p = (mv - self.centre) * 100 / (self.hi - self.centre)
        else:
            p = (mv - self.centre) * 100 / (self.centre - self.lo)
        p = max(-100, min(100, int(p)))
        return 0 if abs(p) < DEAD_ZONE else p   # the dead zone


key = Pin(KEY_PIN, Pin.IN)             # R1 on the board pulls it down
time.sleep_ms(200)
x_axis = Axis(X_PIN)
y_axis = Axis(Y_PIN)

while True:
    print("X %4d  Y %4d" % (x_axis.read(), y_axis.read()))
    time.sleep_ms(100)
