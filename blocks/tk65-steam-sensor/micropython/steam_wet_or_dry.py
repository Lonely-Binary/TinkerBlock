"""
  Steam Sensor - wet or dry, MicroPython                TK65 / /p/tk65

  Wiring. The same as the first reading, except VCC, which comes
  from a pin so the loops only carry current while being read.
  Count from the square pad on the TinkerBlock board, parts up,
  header at the bottom:

    GND    -> GND
    VCC    -> POWER_PIN: GPIO 25 on an ESP32, GPIO 5 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

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
STEAM_PIN = 4
# The GPIO number that powers VCC. ESP32: 25. ESP32-S3: 5. Pico: 15.
POWER_PIN = 5

WET_AT = 65535 * 30 // 100            # wet above 30 % of full scale
DRY_AT = 65535 * 10 // 100            # dry again only below 10 %
SETTLE_MS = 500                       # five of the 0.1 s time constant
PERIOD_MS = 10000                     # one reading every 10 s

adc = ADC(Pin(STEAM_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V

power = Pin(POWER_PIN, Pin.OUT, value=0)   # loops off between reads
wet = False

while True:
    power.on()
    time.sleep_ms(SETTLE_MS)
    reading = adc.read_u16()
    power.off()

    print("reading", reading)
    if not wet and reading > WET_AT:
        wet = True
        print("wet")
    elif wet and reading < DRY_AT:
        wet = False
        print("dry")

    time.sleep_ms(PERIOD_MS - SETTLE_MS)
