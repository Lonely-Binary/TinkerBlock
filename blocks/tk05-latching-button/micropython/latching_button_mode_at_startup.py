"""
  Latching button - a mode chosen at start-up, MicroPython  TK05

  Wiring. Count from the square pad on the TinkerBlock board, switch
  side up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: SIGNAL is VCC when the switch is on,
                     and these are 3.3 V pins)
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
SWITCH_PIN = 4

switch = Pin(SWITCH_PIN, Pin.IN)    # the board's pull-down holds it LOW

# The switch kept its position through the reset. Read it.
test_mode = switch.value() == 1
if test_mode:
    print("Started in TEST mode (switch ON)")
else:
    print("Started in NORMAL mode (switch OFF)")

warned = False
started = time.ticks_ms()
last_report = started

while True:
    now = time.ticks_ms()
    if test_mode and time.ticks_diff(now, last_report) >= 1000:
        last_report = now
        up = time.ticks_diff(now, started) // 1000
        print("test: up for", up, "s")

    # Moved while running? Say so once; it counts from the next reset.
    if (switch.value() == 1) != test_mode and not warned:
        print("Switch moved: press reset to change mode")
        warned = True
