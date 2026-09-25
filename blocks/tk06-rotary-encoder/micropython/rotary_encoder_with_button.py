"""
  Rotary Encoder - count and zero, MicroPython         TK06 / /p/tk06

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
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# CLOCK, DATA and BTN, as GPIO numbers.
# ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
CLOCK_PIN = 4
DATA_PIN = 5
BUTTON_PIN = 6
DEBOUNCE_MS = 20                    # longer than bounce lasts

clock = Pin(CLOCK_PIN, Pin.IN)      # no pull: the block has its own
data = Pin(DATA_PIN, Pin.IN)
button = Pin(BUTTON_PIN, Pin.IN)    # and its own pull-down
count = 0
last_clock = 1
last_reading = 0                    # BTN as last read
pushed = 0                          # BTN as we have decided it is
last_change = time.ticks_ms()

while True:
    level = clock.value()           # the knob, as in the first count
    if level != last_clock:
        if level == 0:
            if data.value() == 1:
                count += 1          # CLOCK fell first
            else:
                count -= 1          # DATA fell first
            print(count)
        last_clock = level

    reading = button.value()        # the shaft switch: 1 is pushed
    now = time.ticks_ms()
    if reading != last_reading:
        last_reading = reading
        last_change = now
    if (time.ticks_diff(now, last_change) >= DEBOUNCE_MS
            and reading != pushed):
        pushed = reading
        if pushed == 1:             # one real push
            count = 0
            print("zero")
