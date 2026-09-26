"""
  Five-Direction Joystick - a menu, MicroPython        TK94 / /p/tk94

  Up and down move through three settings, left and right change
  the one you are on, and a press sets it back to 0. Each push
  counts once, however long you hold it.

  Wiring. Parts up, header at the bottom. Count from the square pad,
  which is GND at the left-hand end:

    GND    -> GND
    VCC    -> 3V3   (never 5V: a push to the right puts VCC on the pin)
    NC     -> nothing: it is connected to nothing on the board
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico                 (analog)

  The names are for the block held with the header on the left,
  the way the words on its front read.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO SIGNAL is wired to.
# ESP32: 34. ESP32-S3: 4. Pico: 26.
SIGNAL_PIN = 4

NAME = ("right", "down", "left", "press", "up")
LIMIT = (722, 359, 227, 146, 56)
RIGHT, DOWN, LEFT, PRESS, UP = range(5)

# A push is believed once it has read the same for SETTLE_MS.
# Letting go takes about 3 ms to fall through the lower levels.
SETTLE_MS = 10
READ_MS = 5

ITEM = ("Speed", "Level", "Volume")
MAX_VALUE = 9
value = [0, 0, 0]
cursor = 0

ESP = sys.platform == "esp32"          # ESP32 and ESP32-S3

adc = ADC(Pin(SIGNAL_PIN))
if ESP:
    adc.atten(ADC.ATTN_11DB)           # the full range, to about 3.1 V


def read_permille():
    if ESP:
        return adc.read_uv() // 3300
    return adc.read_u16() * 1000 // 65535


def which_push(p):
    for i, limit in enumerate(LIMIT):
        if p > limit:
            return i
    return -1


def show():
    print("> %s %d" % (ITEM[cursor], value[cursor]))


def act(d):                            # one push, acted on once
    global cursor
    if d == UP:
        cursor = (cursor - 1) % len(ITEM)
    elif d == DOWN:
        cursor = (cursor + 1) % len(ITEM)
    elif d == LEFT and value[cursor] > 0:
        value[cursor] -= 1
    elif d == RIGHT and value[cursor] < MAX_VALUE:
        value[cursor] += 1
    elif d == PRESS:
        value[cursor] = 0
    show()


candidate = -1                         # what the readings say now
since = time.ticks_ms()                # since when
believed = -1                          # the push acted on
show()

while True:
    d = which_push(read_permille())
    now = time.ticks_ms()
    if d != candidate:                 # changed: start timing it
        candidate = d
        since = now
    elif time.ticks_diff(now, since) >= SETTLE_MS and d != believed:
        believed = d                   # steady, and new
        if d >= 0:                     # letting go is not a push
            act(d)
    time.sleep_ms(READ_MS)
