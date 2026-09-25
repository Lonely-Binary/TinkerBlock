"""
  Rotary Encoder - a state table, MicroPython          TK06 / /p/tk06

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
STEPS_PER_CLICK = 4                 # on many EC11 parts; count yours

# +1 a step forward, -1 a step back, 0 no move or a missed state.
# Index: old state * 4 + new state. A state is CLOCK * 2 + DATA.
TABLE = (0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0)

clock = Pin(CLOCK_PIN, Pin.IN)      # no pull: the block has its own
data = Pin(DATA_PIN, Pin.IN)
state = clock.value() << 1 | data.value()
steps = 0

def on_either(pin):                 # every change of either pin
    global state, steps
    now = clock.value() << 1 | data.value()
    steps += TABLE[state << 2 | now]
    state = now

both = Pin.IRQ_RISING | Pin.IRQ_FALLING
clock.irq(trigger=both, handler=on_either)
data.irq(trigger=both, handler=on_either)

shown = 0
while True:
    clicks = int(steps / STEPS_PER_CLICK)
    if clicks != shown:
        shown = clicks
        print(clicks)
    time.sleep_ms(10)
