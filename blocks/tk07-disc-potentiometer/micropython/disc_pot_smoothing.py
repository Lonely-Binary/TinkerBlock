"""
  Disc Potentiometer - smoothing, MicroPython         TK07 / /p/tk07

  Wiring. Count from the square pad on the TinkerBlock board, wheel
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: at one stop, SIGNAL gives your pin VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    View > Plotter                to see both lines
    Nothing to install: machine, sys and time are built in.
"""

from machine import ADC, Pin
import sys
import time

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
POT_PIN = 4
SAMPLES = 16                        # reads per value

pot = ADC(Pin(POT_PIN))
if sys.platform == "esp32":         # both ESP32s report "esp32"
    pot.atten(ADC.ATTN_11DB)        # measure up to about 3.1 V

def read_averaged():
    total = 0
    for _ in range(SAMPLES):
        total += pot.read_u16()
    return total // SAMPLES

while True:
    raw = pot.read_u16()
    smooth = read_averaged()
    print("raw:", raw, "smooth:", smooth)
    time.sleep_ms(50)
