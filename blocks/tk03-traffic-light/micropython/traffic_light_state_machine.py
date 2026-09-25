"""
  Traffic Light - a state machine, MicroPython        TK03 / /p/tk03

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

red = Pin(RED_PIN, Pin.OUT, value=0)
yellow = Pin(YELLOW_PIN, Pin.OUT, value=0)
green = Pin(GREEN_PIN, Pin.OUT, value=0)

# The whole traffic light. One row per phase, in order.
PHASES = [
    # RED YELLOW GREEN  ms    name
    (1,   0,     0,     5000, "red"),
    (0,   0,     1,     5000, "green"),
    (0,   1,     0,     2000, "yellow"),
]


def show(p):
    r, y, g, _, name = PHASES[p]
    red.value(r)
    yellow.value(y)
    green.value(g)
    print(name)


phase = 0                           # which row is showing
show(phase)
phase_start = time.ticks_ms()       # when it started

while True:
    now = time.ticks_ms()

    # Has this phase had its time? If not, carry straight on.
    if time.ticks_diff(now, phase_start) >= PHASES[phase][3]:
        phase = (phase + 1) % len(PHASES)   # next row; after last, 0
        phase_start = now
        show(phase)

    # Anything else goes here. None of it waits for the light.
