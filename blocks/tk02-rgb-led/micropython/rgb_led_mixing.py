"""
  RGB LED - mixing with PWM, MicroPython              TK02 / /p/tk02

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


def set_colour(r, g, b):
    # 0 is off, 255 is full. Common cathode: no "255 -" anywhere.
    red.duty_u16(r * 257)
    green.duty_u16(g * 257)
    blue.duty_u16(b * 257)


def show(r, g, b):
    set_colour(r, g, b)
    print(r, g, b)
    time.sleep_ms(1500)


while True:
    show(255, 255, 0)           # red and green
    show(0, 255, 255)           # green and blue
    show(255, 0, 255)           # blue and red
    show(255, 60, 0)            # mostly red, a little green
    show(255, 255, 255)         # all three full: look closely
