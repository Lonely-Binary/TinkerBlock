"""
  Rotary Potentiometer - first read, MicroPython      TK08 / /p/tk08

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
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import ADC, Pin
import sys
import time

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
POT_PIN = 4

pot = ADC(Pin(POT_PIN))
if sys.platform == "esp32":         # ESP32 and ESP32-S3 both say esp32
    pot.atten(ADC.ATTN_11DB)        # widest range, roughly 0 to 3.1 V

while True:
    reading = pot.read_u16()        # 0 to 65535
    volts = reading * 3.3 / 65535
    print(reading, "{:.2f} V".format(volts))
    time.sleep_ms(200)              # five lines a second
