"""
  4-Direction Tilt Sensor - first read, MicroPython    TK19 / /p/tk19

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND  -> GND
    VCC  -> 3V3   (never 5V beside a 3.3 V board)
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


def release_all():
    # Every line an input with its pull-up. Never driven HIGH.
    for p in (a, b, c, d):
        p.init(Pin.IN, Pin.PULL_UP)


def hold_low(p):
    p.init(Pin.OUT, value=0)


def scan_tilt():
    release_all()
    hold_low(a)
    time.sleep_us(10)           # let the lines settle
    ab = b.value() == 0         # A-B joined: left edge down
    da = d.value() == 0         # D-A joined: header edge down

    release_all()
    hold_low(c)
    time.sleep_us(10)
    bc = b.value() == 0         # B-C joined: top edge down
    cd = d.value() == 0         # C-D joined: right edge down

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


while True:
    print(scan_tilt())
    time.sleep_ms(200)          # five scans a second
