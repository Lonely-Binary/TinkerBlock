"""
  Push Button - debounced, MicroPython                TK04 / /p/tk04

  Wiring. Count from the square pad on the TinkerBlock board, button
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: pressed, SIGNAL gives your pin VCC)
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
BUTTON_PIN = 4
DEBOUNCE_MS = 20                    # longer than bounce lasts

button = Pin(BUTTON_PIN, Pin.IN)    # no pull: the block has its own
last_reading = 0                    # what the pin said last time round
state = 0                           # what we have decided it is
last_change = time.ticks_ms()
presses = 0

while True:
    reading = button.value()
    now = time.ticks_ms()

    if reading != last_reading:     # the contacts moved: restart
        last_reading = reading
        last_change = now

    # Held still for DEBOUNCE_MS, and different from what we believed?
    if (time.ticks_diff(now, last_change) >= DEBOUNCE_MS
            and reading != state):
        state = reading
        if state == 1:              # one real press
            presses += 1
            print("presses:", presses)
