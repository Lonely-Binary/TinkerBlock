"""
  Piezo-Ceramic Sensor - first reading, MicroPython    TK59 / /p/tk59

  Two wires: the block needs no supply. Count from the square pad on
  the TinkerBlock board, parts up, header on the left:

    GND    -> GND
    NC     -> nothing   (connected to nothing on the board)
    NC     -> nothing   (nor is this one: there is no VCC pin)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    View > Plotter                to see each tap
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
PIEZO_PIN = 4
# A tap is a hump: up in about a millisecond, down over a tenth of a
# second or so. The highest reading in each 20 ms catches the top.
WINDOW_MS = 20

adc = ADC(Pin(PIEZO_PIN))
ESP32 = sys.platform == "esp32"       # ESP32 and ESP32-S3
if ESP32:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V


def read_mv():
    if ESP32:
        return adc.read_uv() // 1000  # calibrated in the chip
    return adc.read_u16() * 3300 // 65535


while True:
    peak = 0
    start = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), start) < WINDOW_MS:
        mv = read_mv()
        if mv > peak:
            peak = mv
    print(peak)
