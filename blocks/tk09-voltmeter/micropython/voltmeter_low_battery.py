"""
  Voltmeter - low-battery warning, MicroPython         TK09 / /p/tk09

  Wiring. Count from the square pad on the TinkerBlock board, terminal
  at the top, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIG    -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Terminal: left screw to the battery's minus, right screw to its plus.

  TK01 XL LED, counted from its own square pad:
    GND    -> GND
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3, GP15 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

from machine import ADC, Pin
import sys
import time

# The GPIO number SIG is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
SIG_PIN = 34
# The pin the TK01 is on. ESP32: 4. ESP32-S3: 5. Pico: 15.
LED_PIN = 4

SCALE = 5.0          # the terminal is 5 x SIG
CAL = 1.000          # multimeter / sketch
SAMPLES = 16
LOW_V = 7.5          # warn below this
HYSTERESIS = 0.2     # and stop only above LOW_V + this

adc = ADC(Pin(SIG_PIN))
led = Pin(LED_PIN, Pin.OUT)

if sys.platform == "rp2":
    def read_once():
        return adc.read_u16() * 3.3 / 65535
else:
    adc.atten(ADC.ATTN_11DB)          # full range; the default is ~1 V
    def read_once():
        return adc.read_uv() / 1_000_000   # calibrated by the firmware

def battery_volts():
    sig = sum(read_once() for _ in range(SAMPLES)) / SAMPLES
    return sig * SCALE * CAL

warning = False
while True:
    battery = battery_volts()
    if not warning and battery < LOW_V:
        warning = True
    elif warning and battery > LOW_V + HYSTERESIS:
        warning = False       # only a real recovery, a new battery
    led.value(1 if warning else 0)
    print("battery {:.2f} V{}".format(battery, "  LOW" if warning else ""))
    time.sleep_ms(1000)
