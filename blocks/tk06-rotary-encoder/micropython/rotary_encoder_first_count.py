"""
  Rotary Encoder - first count, MicroPython            TK06 / /p/tk06

  Wiring. Count from the square pad on the TinkerBlock board, knob
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: CLOCK, DATA and BTN all reach VCC)
    NC     -> nothing   (unconnected on the board)
    BTN    -> GPIO 27 on an ESP32, GPIO 6 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico
    CLOCK  -> GPIO 25, GPIO 4, GP13   (the same three boards)
    DATA   -> GPIO 26, GPIO 5, GP14

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine is built in.
"""

from machine import Pin

# CLOCK, DATA and BTN, as GPIO numbers.
# ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
CLOCK_PIN = 4
DATA_PIN = 5

clock = Pin(CLOCK_PIN, Pin.IN)      # no pull: the block has its own
data = Pin(DATA_PIN, Pin.IN)
count = 0
last_clock = 1                      # at rest, held HIGH

while True:
    level = clock.value()
    if level != last_clock:         # CLOCK has changed...
        if level == 0:              # ...and it has just fallen
            if data.value() == 1:
                count += 1          # CLOCK fell first
            else:
                count -= 1          # DATA fell first
            print(count)
        last_clock = level
