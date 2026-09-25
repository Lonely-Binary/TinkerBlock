"""
  Dual Axis Joystick - eight directions, MicroPython

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
    Nothing to install: machine, math, sys and time are built in.
"""

import math
import sys
import time
from machine import ADC, Pin

# The GPIO numbers X, Y and SW are wired to.
# ESP32: 34 35 25. ESP32-S3: 4 5 7. Pico: 26 27 15.
X_PIN, Y_PIN, SW_PIN = 4, 5, 7

FULL = 65535                # read_u16 on every board
DEAD_ZONE = 10              # as a distance from the centre
EDGE_GUESS = 30             # per cent of full scale, until learned
DEBOUNCE_MS = 20

# From the first read: see the Arduino sketch's comment.
SWAP_XY = False
X_SIGN, Y_SIGN = 1, 1

NAME = ("right", "up-right", "up", "up-left",
        "left", "down-left", "down", "down-right")

ESP = sys.platform == "esp32"          # ESP32 and ESP32-S3


def analog(pin):
    adc = ADC(Pin(pin))
    if ESP:
        adc.atten(ADC.ATTN_11DB)       # the full range, to about 3.1 V
    return adc


def average(adc):
    return sum(adc.read_u16() for _ in range(16)) / 16


class Axis:
    def __init__(self, pin):
        self.adc = analog(pin)
        self.centre = average(self.adc)    # hands off the stick
        guess = FULL * EDGE_GUESS / 100
        self.lo = self.centre - guess
        self.hi = self.centre + guess

    def read(self):
        v = average(self.adc)
        self.lo = min(self.lo, v)
        self.hi = max(self.hi, v)
        if v >= self.centre:
            p = (v - self.centre) * 100 / (self.hi - self.centre)
        else:
            p = (v - self.centre) * 100 / (self.centre - self.lo)
        return max(-100, min(100, int(p)))


def direction(x, y):
    if x * x + y * y < DEAD_ZONE * DEAD_ZONE:
        return -1
    i = round(math.atan2(y, x) / (math.pi / 4))
    return i % 8


sw = Pin(SW_PIN, Pin.IN)               # R10 on the board pulls it down
time.sleep_ms(200)
x_axis = Axis(X_PIN)
y_axis = Axis(Y_PIN)
last_dir, last_sw, quiet_from = -2, 0, time.ticks_ms()

while True:
    a, b = x_axis.read(), y_axis.read()
    x = X_SIGN * (b if SWAP_XY else a)
    y = Y_SIGN * (a if SWAP_XY else b)
    d = direction(x, y)
    if d != last_dir:
        print("centre" if d < 0 else NAME[d])
        last_dir = d
    now = time.ticks_ms()
    if time.ticks_diff(now, quiet_from) >= DEBOUNCE_MS:
        s = sw.value()
        if s != last_sw:
            if s:
                print("click")
            last_sw = s
            quiet_from = now
    time.sleep_ms(10)
