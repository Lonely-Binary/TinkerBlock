"""
  4-Direction Tilt Sensor - lighting the low side     TK19 / /p/tk19

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND  -> GND
    VCC  -> 3V3   (the LEDs need it; the scan does not)
    A    -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3, GP10 on a Pico
    B    -> GPIO 26, GPIO 5, GP11      (same order)
    C    -> GPIO 27, GPIO 6, GP12
    D    -> GPIO 32, GPIO 7, GP13

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO numbers A, B, C and D are wired to.
# ESP32: 25 26 27 32. ESP32-S3: 4 5 6 7. Pico: 10 11 12 13.
PIN_A, PIN_B, PIN_C, PIN_D = 4, 5, 6, 7

a = Pin(PIN_A, Pin.IN, Pin.PULL_UP)
b = Pin(PIN_B, Pin.IN, Pin.PULL_UP)
c = Pin(PIN_C, Pin.IN, Pin.PULL_UP)
d = Pin(PIN_D, Pin.IN, Pin.PULL_UP)

# The two lines on each side.
PAIRS = {"left": (a, b), "top": (b, c), "right": (c, d), "header": (d, a)}


def release_all():
    for p in (a, b, c, d):
        p.init(Pin.IN, Pin.PULL_UP)


def hold_low(p):
    p.init(Pin.OUT, value=0)


def scan_tilt():
    release_all()
    hold_low(a)
    time.sleep_us(10)
    ab = b.value() == 0
    da = d.value() == 0

    release_all()
    hold_low(c)
    time.sleep_us(10)
    bc = b.value() == 0
    cd = d.value() == 0

    release_all()
    if ab:
        return "left"
    if da:
        return "header"
    if bc:
        return "top"
    if cd:
        return "right"
    return "none"


last = None
while True:
    side = scan_tilt()
    for p in PAIRS.get(side, ()):   # light the low side until next time
        hold_low(p)
    if side != last:
        print(side)
        last = side
    time.sleep_ms(20)
