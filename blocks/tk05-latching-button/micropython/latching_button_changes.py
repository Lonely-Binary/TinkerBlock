"""
  Latching button - acting on a change, MicroPython   TK05 / /p/tk05

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
SETTLE_MS = 20                      # a new level must hold this long

switch = Pin(SWITCH_PIN, Pin.IN)    # the board's pull-down holds it LOW
settled = switch.value()            # the level the sketch believes
last_read = settled                 # the pin at the last read
changed_at = time.ticks_ms()        # when last_read last changed
print("Starts ON" if settled else "Starts OFF")

while True:
    now = switch.value()

    if now != last_read:            # it moved, or it bounced
        last_read = now
        changed_at = time.ticks_ms()

    # A change is a new level that has held for SETTLE_MS.
    held = time.ticks_diff(time.ticks_ms(), changed_at)
    if now != settled and held >= SETTLE_MS:
        settled = now
        print("Turned ON" if settled else "Turned OFF")

    # Anything else goes here. Nothing above waits.
