"""
  XL LED - fade with PWM, MicroPython                 TK01 / /p/tk01

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

while True:
    # 0 is always LOW, 65535 is always HIGH; in between, the pin spends
    # that fraction of every PWM period HIGH.
    for duty in range(256):
        led.duty_u16(duty * 257)
        time.sleep_ms(8)
    for duty in range(255, -1, -1):
        led.duty_u16(duty * 257)
        time.sleep_ms(8)
