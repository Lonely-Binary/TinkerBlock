"""
  Vibration Motor - strength with PWM, MicroPython    TK30 / /p/tk30

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
from machine import Pin, PWM

# A PWM pin. ESP32: 4. ESP32-S3: 4. Pico: 15.
MOTOR_PIN = 4

# 0 is off, 255 is full. From full to below the start line.
LEVELS = (255, 200, 150, 100)

motor = PWM(Pin(MOTOR_PIN), freq=1000)
motor.duty_u16(0)

while True:
    for level in LEVELS:
        print("level", level)
        motor.duty_u16(level * 257)   # from rest, every time
        time.sleep_ms(1000)
        motor.duty_u16(0)             # off, and let it stop
        time.sleep_ms(1000)
