"""
  XL LED - a fade that looks even, MicroPython        TK01 / /p/tk01

  Wiring. Count from the square pad on the TinkerBlock board, LED side
  up, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32 or ESP32-S3, GP15 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin, PWM
import time

# A PWM pin. ESP32, ESP32-S3: 4. Pico: 15.
LED_PIN = 4

led = PWM(Pin(LED_PIN), freq=1000)

# 32 steps that look equally spaced: 65535 * (i / 31) ** 2.2, rounded.
LOOKS_EVEN = [round(65535 * (i / 31) ** 2.2) for i in range(32)]

while True:
    for duty in LOOKS_EVEN:
        led.duty_u16(duty)
        time.sleep_ms(60)
    for duty in reversed(LOOKS_EVEN):
        led.duty_u16(duty)
        time.sleep_ms(60)
