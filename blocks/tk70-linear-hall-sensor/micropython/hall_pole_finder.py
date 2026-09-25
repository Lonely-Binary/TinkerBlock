"""
  Linear Hall Effect Sensor - pole finder, MicroPython   TK70 / /p/tk70

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: from 5 V a strong field takes SIGNAL
                     to about 4 V)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Keep magnets away while it starts: it measures its zero then.
  Hold a magnet's face to the front of the board, over the chip.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
HALL_PIN = 4

MV_PER_GAUSS = 2.1    # about, with VCC on 3V3
NORTH_RAISES = True   # the sheet's SOT-23 drawing

# Name a pole past ENTER_G; let it go only under LEAVE_G.
ENTER_G = 20
LEAVE_G = 10

adc = ADC(Pin(HALL_PIN))
ESP = sys.platform == "esp32"         # ESP32 and ESP32-S3
if ESP:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V


def read_millivolts():
    if ESP:
        return adc.read_uv() / 1000   # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


def average_mv(n):
    return sum(read_millivolts() for _ in range(n)) / n


def pole_of(gauss):
    return "north pole" if (gauss > 0) == NORTH_RAISES else "south pole"


time.sleep_ms(500)
zero_mv = average_mv(64)              # no magnet near, please
state = "no magnet"
print(state)

while True:
    gauss = (average_mv(16) - zero_mv) / MV_PER_GAUSS
    strength = abs(gauss)
    new = state
    if strength >= ENTER_G:
        new = pole_of(gauss)
    elif strength < LEAVE_G:
        new = "no magnet"
    if new != state:
        state = new
        print(state)
    if state != "no magnet":
        bar = "#" * min(int(strength / 20), 23)
        print("  %s %.0f G" % (bar, strength))
    time.sleep_ms(200)
