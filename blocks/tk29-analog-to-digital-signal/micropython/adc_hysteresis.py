"""
  Analog to Digital Signal - hysteresis, MicroPython  TK29 / /p/tk29

  Any analog block pushed into IN, parts facing the same way. Either
  switch position: DIG changes at the knob's voltage either way.

  Wiring, OUT to your board. Count from the square pad, parts up,
  OUT at the bottom:

    GND -> GND
    VCC -> 3V3   (never 5V: DIG is pulled up to VCC)
    DIG -> GPIO 25 on an ESP32, GPIO 7 on an ESP32-S3,
           GP15 on a Raspberry Pi Pico
    ANA -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
           GP26 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and sys are built in.
"""

import sys
from machine import ADC, Pin

# DIG_PIN, then ANA_PIN.
# ESP32: 25 and 34. ESP32-S3: 7 and 4. Pico: 15 and 26.
DIG_PIN = 7
ANA_PIN = 4
HYST_MV = 50                          # two lines 100 mV apart

dig = Pin(DIG_PIN, Pin.IN)            # no pull: the block has its own
adc = ADC(Pin(ANA_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V


def read_mv():
    if sys.platform == "esp32":
        return adc.read_uv() / 1000   # calibrated in the chip
    return adc.read_u16() * 3300 / 65535


last = dig.value()
threshold = None                      # learned from DIG
above = False
flips = 0

while True:
    mv = read_mv()
    now = dig.value()
    if now != last:                   # DIG changes only at the knob
        last = now
        threshold = mv
        flips += 1
    if threshold is None:
        continue                      # not crossed yet
    was = above
    if not above and mv > threshold + HYST_MV:
        above = True
    elif above and mv < threshold - HYST_MV:
        above = False
    if above != was:
        print("above" if above else "below", "the knob;",
              "DIG flipped", flips, "times")
        flips = 0
