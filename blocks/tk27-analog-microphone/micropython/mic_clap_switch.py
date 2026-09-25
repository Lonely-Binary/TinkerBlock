"""
  Analog Microphone - a clap switch, MicroPython       TK27 / /p/tk27

  Wiring, the TK27. Count from the square pad, parts up, header at
  the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: a loud sound takes SIGNAL up to VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

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
MIC_PIN = 4
# The TK01's SIGNAL. ESP32: 4. ESP32-S3: 5. Pico: 15.
LED_PIN = 5

WINDOW_MS = 50
CALIBRATE_MS = 1000
CLAP_MARGIN_MV = 200    # how much louder than quiet
LOCKOUT_MS = 300        # deaf after each switch

adc = ADC(Pin(MIC_PIN))
ESP = sys.platform == "esp32"         # ESP32 and ESP32-S3
if ESP:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V
led = Pin(LED_PIN, Pin.OUT)


def read_millivolts():
    if ESP:
        return adc.read_uv() / 1000   # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


def swing_millivolts():
    lo = 100000
    hi = -1
    start = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), start) < WINDOW_MS:
        mv = read_millivolts()
        lo = min(lo, mv)
        hi = max(hi, mv)
    return hi - lo


time.sleep_ms(500)
print("Measuring the quiet swing. Keep quiet...")
quiet = 0
start = time.ticks_ms()
while time.ticks_diff(time.ticks_ms(), start) < CALIBRATE_MS:
    quiet = max(quiet, swing_millivolts())
threshold = quiet + CLAP_MARGIN_MV
print("Clap above %.0f mV" % threshold)

lamp_on = False
while True:
    swing = swing_millivolts()
    if swing > threshold:
        lamp_on = not lamp_on
        led.value(1 if lamp_on else 0)
        print("%.0f mV  %s" % (swing, "on" if lamp_on else "off"))
        time.sleep_ms(LOCKOUT_MS)   # let the clap and its echo die
