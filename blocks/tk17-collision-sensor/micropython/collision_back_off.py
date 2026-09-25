"""
  Collision Sensor - stop and back off, MicroPython   TK17 / /p/tk17

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
DEBOUNCE_MS = 20
BACK_MS = 400                       # reverse for this long
TURN_MS = 300                       # then turn for this long

bumper = Pin(BUMPER_PIN, Pin.IN)    # no pull: the block has its own
last_reading = 0
state = 0                           # the debounced reading
last_change = time.ticks_ms()

# Your motor driver's calls go in these three.
def forward():
    print("forward")

def reverse():
    print("reverse")

def turn_left():
    print("turn left")

def hit():
    """True once per hit: the debounced reading has just become 1."""
    global last_reading, state, last_change
    reading = bumper.value()
    now = time.ticks_ms()
    if reading != last_reading:
        last_reading = reading
        last_change = now
    if (time.ticks_diff(now, last_change) >= DEBOUNCE_MS
            and reading != state):
        state = reading
        return state == 1
    return False

mode = "DRIVE"
mode_since = time.ticks_ms()
forward()

while True:
    in_mode = time.ticks_diff(time.ticks_ms(), mode_since)
    if hit():                       # in any mode: back off, now
        mode, mode_since = "BACK", time.ticks_ms()
        reverse()
    elif mode == "BACK" and in_mode >= BACK_MS:
        mode, mode_since = "TURN", time.ticks_ms()
        turn_left()
    elif mode == "TURN" and in_mode >= TURN_MS:
        mode, mode_since = "DRIVE", time.ticks_ms()
        forward()
