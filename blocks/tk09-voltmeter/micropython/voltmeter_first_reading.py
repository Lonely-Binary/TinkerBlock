"""
  Voltmeter - first reading, MicroPython               TK09 / /p/tk09

  Wiring. Count from the square pad on the TinkerBlock board, terminal
  at the top, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIG    -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Terminal, same way up. Nothing is printed on it:
    left screw   GND: the minus side of what you measure
    right screw  +:   the plus side. DC only. At most about 15.5 V
                      on an ESP32 or ESP32-S3, 16.5 V on a Pico.
                      Never mains.

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

SCALE = 5.0            # the board's divider: the terminal is 5 x SIG

adc = ADC(Pin(SIG_PIN))

if sys.platform == "rp2":
    def sig_volts():
        return adc.read_u16() * 3.3 / 65535
else:
    adc.atten(ADC.ATTN_11DB)          # full range; the default is ~1 V
    def sig_volts():
        return adc.read_uv() / 1_000_000   # calibrated by the firmware

while True:
    sig = sig_volts()
    print("SIG {:.3f} V   input {:.2f} V".format(sig, sig * SCALE))
    time.sleep_ms(500)
