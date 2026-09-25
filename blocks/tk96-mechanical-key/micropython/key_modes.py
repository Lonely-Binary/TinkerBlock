"""
  Mechanical Key and LED - a key that remembers          TK96 / /p/tk96

  Each press moves to the next mode, and the key shows the mode it
  is in: off, red, green, blue, then off again.

  Wiring. Count from the square pad on the TinkerBlock board, key
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: pressed, BUTTON gives your pin VCC)
    WS2812 -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3,
              GP14 on a Raspberry Pi Pico
    BUTTON -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, neopixel and time are built in.
"""

from machine import Pin
from neopixel import NeoPixel
import time

# ESP32: 25 and 4. ESP32-S3: 4 and 5. Pico: 15 and 14.
BUTTON_PIN = 4
LED_PIN = 5
DEBOUNCE_MS = 20

COLOUR = [(0, 0, 0), (255, 0, 0), (0, 255, 0), (0, 0, 255)]
NAME = ["off", "red", "green", "blue"]

button = Pin(BUTTON_PIN, Pin.IN)    # no pull: the block has its own
key = NeoPixel(Pin(LED_PIN), 1)
key[0] = COLOUR[0]
key.write()                         # a dark frame: mode 0 is off

mode = 0                            # what the key remembers
last_reading = 0
state = 0
last_change = time.ticks_ms()

while True:
    reading = button.value()
    now = time.ticks_ms()
    if reading != last_reading:     # the contacts moved: restart
        last_reading = reading
        last_change = now
    settled = time.ticks_diff(now, last_change) >= DEBOUNCE_MS
    if settled and reading != state:
        state = reading
        if state == 1:              # one real press: next mode
            mode = (mode + 1) % len(COLOUR)
            key[0] = COLOUR[mode]
            key.write()
            print("mode", mode, "", NAME[mode])
