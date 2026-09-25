"""
  Rotary Potentiometer - the sweep, MicroPython       TK08 / /p/tk08

  Wiring. Count from the square pad on the TinkerBlock board, knob
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: the full turn puts VCC on your pin)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    View > Plotter                draws the printed numbers
    Nothing to install: machine and time are built in.
"""

from machine import ADC, Pin
import sys
import time

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
POT_PIN = 4

pot = ADC(Pin(POT_PIN))
if sys.platform == "esp32":
    pot.atten(ADC.ATTN_11DB)        # widest range, roughly 0 to 3.1 V

while True:
    print("reading:", pot.read_u16() >> 4)   # 0 to 4095
    time.sleep_ms(20)                         # fifty points a second
