"""
  Voltmeter - calibrated and averaged, MicroPython     TK09 / /p/tk09

  Wiring. Count from the square pad on the TinkerBlock board, terminal
  at the top, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIG    -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Terminal, same way up: left screw GND, right screw +. DC only, at
  most about 15.5 V on an ESP32 or ESP32-S3, 16.5 V on a Pico. Never
  mains.

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

SCALE = 5.0      # 30 k over 7.5 k: the terminal is 5 x SIG
CAL = 1.000      # multimeter / sketch, once you have both
SAMPLES = 16     # averaging 16 leaves a quarter of the wander

adc = ADC(Pin(SIG_PIN))

if sys.platform == "rp2":
    def read_once():
        return adc.read_u16() * 3.3 / 65535
else:
    adc.atten(ADC.ATTN_11DB)          # full range; the default is ~1 V
    def read_once():
        return adc.read_uv() / 1_000_000   # calibrated by the firmware

def sig_volts():
    return sum(read_once() for _ in range(SAMPLES)) / SAMPLES

while True:
    print("input {:.2f} V".format(sig_volts() * SCALE * CAL))
    time.sleep_ms(500)
