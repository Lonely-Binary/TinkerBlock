"""
  Infrared Transmitter - first blink, MicroPython       TK63 / /p/tk63

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V (VBUS on a Pico) for full range; 3V3 works, shorter
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 6 on an ESP32-S3, GPIO 22 on an ESP32,
              GP17 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import Pin

# The GPIO number SIGNAL is wired to. ESP32: 22. ESP32-S3: 6. Pico: 17.
IR_PIN = 6

ir = Pin(IR_PIN, Pin.OUT, value=0)   # LOW: both LEDs off

while True:
    ir.value(1)                      # transistor on: both LEDs light
    print("on: the red LED is lit; look through a camera")
    time.sleep(1)
    ir.value(0)
    print("off")
    time.sleep(1)
