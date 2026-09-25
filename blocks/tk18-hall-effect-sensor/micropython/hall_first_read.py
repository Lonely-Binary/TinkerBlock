"""
  Hall Effect Sensor - first read, MicroPython        TK18 / /p/tk18

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: with no magnet, SIGNAL sits at VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

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

while True:
    if hall.value() == 0:           # active low: 0 is a magnet
        print("LOW   magnet")
    else:
        print("HIGH  no magnet")
    time.sleep_ms(200)              # five reads a second
