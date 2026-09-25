"""
  Mechanical Key and LED - the first press, MicroPython  TK96 / /p/tk96

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

button = Pin(BUTTON_PIN, Pin.IN)    # no pull: the block has its own
key = NeoPixel(Pin(LED_PIN), 1)     # one LED; it sends green first
key[0] = (0, 0, 0)
key.write()                         # a dark frame: it starts unset

last = 0
while True:
    level = button.value()
    if level != last:               # print only when it changes
        last = level
        if level == 1:
            key[0] = (255, 0, 0)    # red
            print("HIGH  pressed")
        else:
            key[0] = (0, 0, 0)      # off
            print("LOW   released")
        key.write()                 # nothing changes until write()
    time.sleep_ms(5)
