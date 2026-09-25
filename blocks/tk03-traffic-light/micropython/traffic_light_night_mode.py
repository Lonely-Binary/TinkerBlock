"""
  Traffic Light - night mode, MicroPython             TK03 / /p/tk03

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
    Type n or d in the shell while it runs, then press Enter.
    Nothing to install: machine, time, sys and select are built in.
"""

from machine import Pin
import select
import sys
import time

# GPIO numbers. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
RED_PIN = 4
YELLOW_PIN = 5
GREEN_PIN = 6

red = Pin(RED_PIN, Pin.OUT, value=0)
yellow = Pin(YELLOW_PIN, Pin.OUT, value=0)
green = Pin(GREEN_PIN, Pin.OUT, value=0)

PHASES = [
    # RED YELLOW GREEN  ms    name
    (1,   0,     0,     5000, "red"),
    (0,   0,     1,     5000, "green"),
    (0,   1,     0,     2000, "yellow"),
]
FLASH_MS = 500                      # night: 500 ms on, 500 ms off


def lights(r, y, g):
    red.value(r)
    yellow.value(y)
    green.value(g)


def show(p):
    r, y, g, _, name = PHASES[p]
    lights(r, y, g)
    print(name)


keys = select.poll()
keys.register(sys.stdin, select.POLLIN)

phase = 0
show(phase)
phase_start = time.ticks_ms()
night = False                       # the second piece of state
flash_start = 0

while True:
    now = time.ticks_ms()

    # A command, if one has arrived. poll(0) never waits for one.
    if keys.poll(0):
        c = sys.stdin.read(1)
        if c == "n" and not night:
            night = True
            flash_start = now
            print("night: flashing yellow")
        elif c == "d" and night:
            night = False
            phase = 0                   # day always restarts on red
            phase_start = now
            show(phase)

    if night:
        gone = time.ticks_diff(now, flash_start)
        lights(0, 1 if (gone // FLASH_MS) % 2 == 0 else 0, 0)
    elif time.ticks_diff(now, phase_start) >= PHASES[phase][3]:
        phase = (phase + 1) % len(PHASES)
        phase_start = now
        show(phase)
