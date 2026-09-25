"""
  Latching button - first read, MicroPython           TK05 / /p/tk05

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

while True:
    if switch.value() == 1:         # switch on: SIGNAL is VCC
        print("SIGNAL is HIGH: the switch is ON")
    else:                           # switch off: 0 V
        print("SIGNAL is LOW: the switch is OFF")
    time.sleep_ms(500)
