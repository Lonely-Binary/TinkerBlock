"""
  RGB LED - a colour wheel, MicroPython               TK02 / /p/tk02

  Wiring. Count from the square pad on the TinkerBlock board, LED side
  up, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    NC     -> nothing
    RED    -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP13 on a Raspberry Pi Pico
    GREEN  -> GPIO 26, GPIO 5, GP14
    BLUE   -> GPIO 27, GPIO 6, GP15

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin, PWM
import time

# ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
red = PWM(Pin(4), freq=1000)
green = PWM(Pin(5), freq=1000)
blue = PWM(Pin(6), freq=1000)

STEP_MS = 20                    # per degree: 7.2 s per lap


def set_colour(r, g, b):
    red.duty_u16(r * 257)
    green.duty_u16(g * 257)
    blue.duty_u16(b * 257)


def wheel(hue):
    # hue 0 to 359: red at 0, green at 120, blue at 240.
    sector = hue // 120
    up = (hue % 120) * 255 // 120   # 0 to 252 across the sector
    down = 255 - up
    if sector == 0:
        set_colour(down, up, 0)     # red to green
    elif sector == 1:
        set_colour(0, down, up)     # green to blue
    else:
        set_colour(up, 0, down)     # blue to red


hue = 0
last = time.ticks_ms()

while True:
    if time.ticks_diff(time.ticks_ms(), last) >= STEP_MS:
        last = time.ticks_add(last, STEP_MS)
        wheel(hue)
        hue = (hue + 1) % 360       # 359 wraps to 0, never 360
    # anything else the program does goes here, and never waits
