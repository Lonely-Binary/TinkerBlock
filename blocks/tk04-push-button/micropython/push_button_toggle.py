"""
  Push Button - a toggle, MicroPython                 TK04 / /p/tk04

  Wiring. Count from the square pad on the TinkerBlock board, button
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: pressed, SIGNAL gives your pin VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  A TK01 XL LED on a second pin shows the state (optional):
    GND    -> GND
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3, GP14 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO number the button's SIGNAL is wired to.
# ESP32: 25. ESP32-S3: 4. Pico: 15.
BUTTON_PIN = 4
# The GPIO number the TK01's SIGNAL is wired to.
# ESP32: 4. ESP32-S3: 5. Pico: 14.
LED_PIN = 5
DEBOUNCE_MS = 20

button = Pin(BUTTON_PIN, Pin.IN)    # no pull: the block has its own
led = Pin(LED_PIN, Pin.OUT, value=0)
last_reading = 0
state = 0
last_change = time.ticks_ms()
led_on = False                      # the toggle's memory

while True:
    reading = button.value()
    now = time.ticks_ms()

    if reading != last_reading:     # the contacts moved: restart
        last_reading = reading
        last_change = now

    if (time.ticks_diff(now, last_change) >= DEBOUNCE_MS
            and reading != state):
        state = reading
        if state == 1:              # one real press: flip
            led_on = not led_on
            led.value(led_on)
            print("on" if led_on else "off")
