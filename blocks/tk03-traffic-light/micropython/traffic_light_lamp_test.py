"""
  Traffic Light - lamp test, MicroPython              TK03 / /p/tk03

  Wiring. Count from the square pad on the TinkerBlock board, LEDs up,
  header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    RED    -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3, GP13 on a Pico
    YELLOW -> GPIO 26 on an ESP32, GPIO 5 on an ESP32-S3, GP14 on a Pico
    GREEN  -> GPIO 27 on an ESP32, GPIO 6 on an ESP32-S3, GP15 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# GPIO numbers. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
RED_PIN = 4
YELLOW_PIN = 5
GREEN_PIN = 6

red = Pin(RED_PIN, Pin.OUT, value=0)       # each light is its own output
yellow = Pin(YELLOW_PIN, Pin.OUT, value=0)
green = Pin(GREEN_PIN, Pin.OUT, value=0)
lights = [(red, "RED"), (yellow, "YELLOW"), (green, "GREEN")]

while True:
    for pin, name in lights:        # one at a time, and say which
        pin.value(1)
        print(name)
        time.sleep_ms(1000)
        pin.value(0)

    for pin, _ in lights:           # all three: allowed, never real
        pin.value(1)
    print("all three")
    time.sleep_ms(1000)

    for pin, _ in lights:
        pin.value(0)
    print("off")
    time.sleep_ms(1000)
