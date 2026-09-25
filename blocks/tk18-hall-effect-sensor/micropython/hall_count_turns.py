"""
  Hall Effect Sensor - counting turns, MicroPython    TK18 / /p/tk18

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: with no magnet, SIGNAL sits at VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  One magnet on the wheel, a flat face passing the small chip at
  the top left of the block.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
HALL_PIN = 4

hall = Pin(HALL_PIN, Pin.IN)        # the chip drives SIGNAL both ways
last_magnet = False
turns = 0
last_pass = None

while True:
    magnet = hall.value() == 0      # active low
    if magnet and not last_magnet:  # the magnet has just arrived
        now = time.ticks_ms()
        turns += 1
        if last_pass is None:
            print("turns:", turns)
        else:
            gap = time.ticks_diff(now, last_pass)
            print("turns:", turns, "  rpm:", round(60000 / gap))
        last_pass = now
    last_magnet = magnet
