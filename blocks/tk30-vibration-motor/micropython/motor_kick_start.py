"""
  Vibration Motor - kick-start, MicroPython           TK30 / /p/tk30

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

KICK_MS = 50                  # full power first: a guess, tune it

motor = PWM(Pin(MOTOR_PIN), freq=1000)
motor.duty_u16(0)
running = False

def set_level(level):
    """Any strength, 0 to 255. From rest, kick it first."""
    global running
    if level > 0 and not running:
        motor.duty_u16(65535)
        time.sleep_ms(KICK_MS)
    motor.duty_u16(level * 257)
    running = level > 0

while True:
    for level in (150, 100, 70):
        print("level", level)
        set_level(level)
        time.sleep_ms(1500)
        set_level(0)
        time.sleep_ms(1000)
