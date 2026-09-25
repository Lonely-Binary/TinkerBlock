"""
  Slim Joystick - first read, MicroPython           TK21 / /p/tk21

  Wiring. Parts up, header along the top. Count from the square pad,
  which is GND at the right-hand end, leftwards:

    GND  -> GND
    3V3  -> 3V3   (the header is printed 3V3; never 5V)
    BTNS -> GPIO 32 on an ESP32, GPIO 6 on an ESP32-S3,
            GP28 on a Raspberry Pi Pico          (analog)
    X    -> GPIO 34, GPIO 4, GP26      (same order, analog)
    Y    -> GPIO 35, GPIO 5, GP27      (analog)
    KEY  -> GPIO 25, GPIO 7, GP15      (digital)

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO numbers X, Y, BTNS and KEY are wired to.
# ESP32: 34 35 32 25. ESP32-S3: 4 5 6 7. Pico: 26 27 28 15.
X_PIN, Y_PIN, BTNS_PIN, KEY_PIN = 4, 5, 6, 7

# The buttons in ladder order, and the line under each one's level,
# in mV with 3V3 on the header.
BUTTON_NAME = ("U11", "U10", "U5", "U7", "U8", "U6")
LIMIT_MV = (2475, 1375, 963, 743, 605, 275)

ESP = sys.platform == "esp32"          # ESP32 and ESP32-S3


def analog(pin):
    adc = ADC(Pin(pin))
    if ESP:
        adc.atten(ADC.ATTN_11DB)       # the full range, to about 3.1 V
    return adc


x_adc = analog(X_PIN)
y_adc = analog(Y_PIN)
btns = analog(BTNS_PIN)
key = Pin(KEY_PIN, Pin.IN)             # R1 on the board pulls it down


def millivolts(adc):
    if ESP:
        return adc.read_uv() / 1000    # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


def which_button(mv):
    for i, limit in enumerate(LIMIT_MV):
        if mv > limit:
            return i
    return -1


while True:
    mv = millivolts(btns)
    b = which_button(mv)
    print("X %5d  Y %5d  KEY %d  BTNS %4.0f mV  %s" % (
        x_adc.read_u16(), y_adc.read_u16(), key.value(), mv,
        "none" if b < 0 else BUTTON_NAME[b]))
    time.sleep_ms(200)                 # five lines a second
