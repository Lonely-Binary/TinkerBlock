"""
  Collision Sensor - first read, MicroPython          TK17 / /p/tk17

  Wiring. Count from the square pad on the TinkerBlock board, switch
  at the top, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: on a hit, SIGNAL gives your pin VCC)
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
BUMPER_PIN = 4

bumper = Pin(BUMPER_PIN, Pin.IN)    # no pull: the block has its own

while True:
    if bumper.value() == 1:
        print("HIGH  hit")
    else:
        print("LOW   clear")
    time.sleep_ms(200)              # five reads a second
