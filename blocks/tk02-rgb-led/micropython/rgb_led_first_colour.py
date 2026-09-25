"""
  RGB LED - the first colour, MicroPython             TK02 / /p/tk02

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

from machine import Pin
import time

# ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
red = Pin(4, Pin.OUT)
green = Pin(5, Pin.OUT)
blue = Pin(6, Pin.OUT)


def show(r, g, b, name):
    red.value(r)                # 1 lights: common cathode
    green.value(g)
    blue.value(b)
    print(name)
    time.sleep_ms(1000)


while True:
    show(1, 0, 0, "red")
    show(0, 1, 0, "green")
    show(0, 0, 1, "blue")
    show(0, 0, 0, "off")
