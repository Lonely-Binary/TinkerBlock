"""
  XL LED - first blink, MicroPython                   TK01 / /p/tk01

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

from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32, ESP32-S3: 4. Pico: 15.
LED_PIN = 4

led = Pin(LED_PIN, Pin.OUT)     # an output, like pinMode(OUTPUT)

while True:
    led.value(1)                # the pin supplies the LED's current
    print("on")
    time.sleep_ms(500)

    led.value(0)                # 0 V on the anode, nothing flows
    print("off")
    time.sleep_ms(500)
