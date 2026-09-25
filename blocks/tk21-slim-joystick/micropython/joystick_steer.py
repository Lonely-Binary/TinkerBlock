"""
  Slim Joystick - steering a dot, MicroPython         TK21 / /p/tk21

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

# The buttons in ladder order, and the line under each one's level.
BUTTON_NAME = ("U11", "U10", "U5", "U7", "U8", "U6")
LIMIT_MV = (2475, 1375, 963, 743, 605, 275)

DEAD_ZONE = 10              # per cent either side of the centre
EDGE_GUESS_MV = 1000        # the ends, until the stick shows them
FIELD_W, FIELD_H = 21, 9
STEP_PER = 50               # per cent of tilt per cell a frame
FRAME_MS = 100
FLIP_X = False              # True if the dot goes the wrong way
FLIP_Y = False

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


btns = analog(BTNS_PIN)
key = Pin(KEY_PIN, Pin.IN)             # R1 on the board pulls it down
time.sleep_ms(200)
x_axis = Axis(X_PIN)
y_axis = Axis(Y_PIN)


def which_button():
    mv = millivolts(btns)
    for i, limit in enumerate(LIMIT_MV):
        if mv > limit:
            return BUTTON_NAME[i]
    return None


def clamp(v, top):
    return max(0, min(top, v))


dot_x, dot_y = FIELD_W // 2, FIELD_H // 2
marks = set()
last_button = None
last_key = 0

while True:
    sx, sy = x_axis.read(), y_axis.read()
    if FLIP_X:
        sx = -sx
    if FLIP_Y:
        sy = -sy
    dot_x = clamp(dot_x + int(sx / STEP_PER), FIELD_W - 1)
    dot_y = clamp(dot_y - int(sy / STEP_PER), FIELD_H - 1)

    b = which_button()
    if b is not None and b != last_button:   # once per press
        if b == "U11":
            dot_x, dot_y = FIELD_W // 2, FIELD_H // 2
        elif b == "U10":
            marks.clear()
        elif b == "U5":
            dot_y = clamp(dot_y + 1, FIELD_H - 1)    # down
        elif b == "U7":
            dot_x = clamp(dot_x - 1, FIELD_W - 1)    # left
        elif b == "U8":
            dot_y = clamp(dot_y - 1, FIELD_H - 1)    # up
        elif b == "U6":
            dot_x = clamp(dot_x + 1, FIELD_W - 1)    # right
    last_button = b

    k = key.value()                    # 1 while clicked
    if k and not last_key:
        marks.add((dot_x, dot_y))
    last_key = k

    print()
    for y in range(FIELD_H):
        row = ""
        for x in range(FIELD_W):
            if (x, y) == (dot_x, dot_y):
                row += "@"
            elif (x, y) in marks:
                row += "+"
            else:
                row += "."
        print(row)
    time.sleep_ms(FRAME_MS)
