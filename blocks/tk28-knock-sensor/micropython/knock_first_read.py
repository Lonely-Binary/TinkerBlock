"""
  Knock Sensor - first read, MicroPython             TK28 / /p/tk28

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

knock = Pin(KNOCK_PIN, Pin.IN)      # no pull: the block has its own
last = 0                            # the reading last time round
pulses = 0

while True:
    level = knock.value()
    if level == 1 and last == 0:    # a change to HIGH: a pulse
        pulses += 1
        print(time.ticks_ms(), "ms  pulse", pulses)
    last = level                    # never waits: a pulse is about 1 ms
