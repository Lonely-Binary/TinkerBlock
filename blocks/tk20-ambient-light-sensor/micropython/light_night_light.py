"""
  Ambient Light Sensor - a night light, MicroPython   TK20 / /p/tk20

  Wiring, the TK20. Count from the square pad, parts up, header at
  the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: bright light takes SIGNAL towards VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way. Point it away from the TK20:

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
LIGHT_PIN = 4
# The TK01's SIGNAL. ESP32: 4. ESP32-S3: 5. Pico: 15.
LED_PIN = 5

WINDOW_MS = 50      # 5 cycles of a 100 Hz ripple, 6 of a 120 Hz one
ON_FRACTION = 0.4   # on below 40 % of dark-to-lit
OFF_FRACTION = 0.6  # off only above 60 %

adc = ADC(Pin(LIGHT_PIN))
ESP = sys.platform == "esp32"         # ESP32 and ESP32-S3
if ESP:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V
led = Pin(LED_PIN, Pin.OUT)


def read_millivolts():
    if ESP:
        return adc.read_uv() / 1000   # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


def read_steady_millivolts():
    total = 0
    n = 0
    start = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), start) < WINDOW_MS:
        total += read_millivolts()
        n += 1
    return total / n


time.sleep_ms(1000)
print("Calibrating. Leave the room lit...")
time.sleep_ms(3000)
lit = read_steady_millivolts()
print("Now cover the sensor with a finger...")
time.sleep_ms(4000)
dark = read_steady_millivolts()
print("Done. Uncover it.")
if lit - dark < 20:
    print("Lit and dark are too close: stop and run it again.")
on_below = dark + ON_FRACTION * (lit - dark)
off_above = dark + OFF_FRACTION * (lit - dark)
print("On below %.0f mV, off above %.0f mV" % (on_below, off_above))

lamp_on = False
while True:
    mv = read_steady_millivolts()
    if not lamp_on and mv < on_below:
        lamp_on = True
    if lamp_on and mv > off_above:
        lamp_on = False
    led.value(1 if lamp_on else 0)
    print("%.0f mV%s" % (mv, "  on" if lamp_on else ""))
    time.sleep_ms(200)
