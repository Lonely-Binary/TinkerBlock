"""
  Five-Direction Joystick - first read, MicroPython    TK94 / /p/tk94

  Wiring. Parts up, header at the bottom. Count from the square pad,
  which is GND at the left-hand end:

    GND    -> GND
    VCC    -> 3V3   (never 5V: a push to the right puts VCC on the pin)
    NC     -> nothing: it is connected to nothing on the board
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico                 (analog)

  The names are for the block held with the header on the left,
  the way the words on its front read.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO SIGNAL is wired to.
# ESP32: 34. ESP32-S3: 4. Pico: 26.
SIGNAL_PIN = 4

# The pushes, highest level first, and the line under each one:
# halfway to the next level down, in thousandths of VCC.
NAME = ("right", "down", "left", "press", "up")
LIMIT = (722, 359, 227, 146, 56)

ESP = sys.platform == "esp32"          # ESP32 and ESP32-S3

adc = ADC(Pin(SIGNAL_PIN))
if ESP:
    adc.atten(ADC.ATTN_11DB)           # the full range, to about 3.1 V


def read_permille():
    if ESP:
        return adc.read_uv() // 3300   # calibrated, over VCC's 3.3 V
    return adc.read_u16() * 1000 // 65535


def which_push(p):
    for i, limit in enumerate(LIMIT):
        if p > limit:
            return i
    return -1


while True:
    p = read_permille()
    d = which_push(p)
    print("SIGNAL %4d/1000  %s" % (p, "none" if d < 0 else NAME[d]))
    time.sleep_ms(200)                 # five lines a second
