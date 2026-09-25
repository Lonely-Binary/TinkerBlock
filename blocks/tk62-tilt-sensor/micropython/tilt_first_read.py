"""
  Tilt Sensor - first read, MicroPython                 TK62 / /p/tk62

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (your board's logic voltage; never 5V beside
                     a 3.3 V board)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 25 on an ESP32,
              GP15 on a Raspberry Pi Pico

  The can's leads are at the board's left edge. Left edge down:
  switch closed, 1. Right edge down: open, 0. Near level it keeps
  whatever it last read.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO SIGNAL is wired to.
# ESP32: 25. ESP32-S3: 4. Pico: 15.
TILT_PIN = 4

# Pin.IN with no pull: the board has its own 10k pull-down.
tilt = Pin(TILT_PIN, Pin.IN)

last = None
while True:
    level = tilt.value()
    if level != last:
        if level:
            print("left edge down: switch closed")
        else:
            print("right edge down: switch open")
        last = level
    time.sleep_ms(10)
