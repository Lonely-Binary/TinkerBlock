"""
  Knock Sensor - counting knocks, with a hold-off     TK28 / /p/tk28

  Wiring. Count from the square pad on the TinkerBlock board, switch
  at the top, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: during a knock, SIGNAL gives your pin VCC)
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
KNOCK_PIN = 4
HOLD_OFF_MS = 100                   # longer than the spring rings

knock = Pin(KNOCK_PIN, Pin.IN)      # no pull: the block has its own
last = 0                            # the reading last time round
last_knock = time.ticks_add(time.ticks_ms(), -HOLD_OFF_MS)
knocks = 0

while True:
    level = knock.value()
    now = time.ticks_ms()
    if level == 1 and last == 0:    # a pulse starts
        if time.ticks_diff(now, last_knock) >= HOLD_OFF_MS:
            knocks += 1             # not the same knock
            last_knock = now
            print("knocks:", knocks)
    last = level
