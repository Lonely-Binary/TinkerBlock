"""
  XL LED - blinking without sleep(), MicroPython      TK01 / /p/tk01

  Wiring. Count from the square pad on the TinkerBlock board, LED side
  up, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32 or ESP32-S3, GP15 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32, ESP32-S3: 4. Pico: 15.
LED_PIN = 4
INTERVAL_MS = 500               # on for 500 ms, off for 500 ms

led = Pin(LED_PIN, Pin.OUT)
last_change = time.ticks_ms()
passes = 0

while True:
    now = time.ticks_ms()

    # Has half a second gone by since the LED last changed?
    if time.ticks_diff(now, last_change) >= INTERVAL_MS:
        last_change = now
        led.value(not led.value())
        if led.value():             # once a second, report and reset
            print("loop ran", passes, "times in the last second")
            passes = 0

    passes += 1
    # Anything else goes here: read a button, a sensor, the serial port.
    # None of it waits for the LED.
