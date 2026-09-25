"""
  RGB LED - equal numbers against white, MicroPython  TK02 / /p/tk02

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

# A first guess at white from a 3.3 V board: 84, 255, 122.
WHITE = (84, 255, 122)


def set_colour(r, g, b):
    red.duty_u16(r * 257)
    green.duty_u16(g * 257)
    blue.duty_u16(b * 257)


while True:
    set_colour(255, 255, 255)
    print("255, 255, 255: equal numbers")
    time.sleep_ms(2000)

    set_colour(*WHITE)
    print("WHITE: balanced")
    time.sleep_ms(2000)
