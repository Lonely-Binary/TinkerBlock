"""
  Vibration Motor - first buzz, MicroPython           TK30 / /p/tk30

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: the 5V pin of an ESP32 or ESP32-S3 board on USB,
              a Pico's VBUS (3V3 works as well)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32 or ESP32-S3, GP15 on a Raspberry
              Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

import time
from machine import Pin

# The GPIO number SIGNAL is wired to. ESP32: 4. ESP32-S3: 4. Pico: 15.
MOTOR_PIN = 4

motor = Pin(MOTOR_PIN, Pin.OUT, value=0)

while True:
    motor.on()                # the MOSFET closes: buzz
    print("on")
    time.sleep_ms(500)
    motor.off()               # open: the motor coasts to a stop
    print("off")
    time.sleep_ms(500)
