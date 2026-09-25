"""
  Vibration Motor - patterns on a button, MicroPython  TK30 / /p/tk30

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: the 5V pin of an ESP32 or ESP32-S3 board on USB,
              a Pico's VBUS (3V3 works as well)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32 or ESP32-S3, GP15 on a Raspberry
              Pi Pico

  TK04 Push Button: GND to GND, VCC to 3V3, SIGNAL to GPIO 25 on an
  ESP32, GPIO 5 on an ESP32-S3, GP14 on a Pico.

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
# The TK04's SIGNAL. ESP32: 25. ESP32-S3: 5. Pico: 14.
BUTTON_PIN = 5

# On, off, on ... in ms, on first.
TAP_MS = (100,)
DOUBLE_MS = (100, 120, 100)
LONG_MS = (400,)
ALARM_MS = (200, 200, 200, 200, 200)

READ_MS = 20                   # read the button this often

motor = Pin(MOTOR_PIN, Pin.OUT, value=0)
button = Pin(BUTTON_PIN, Pin.IN)   # the TK04 has its own pull-down

pattern = None
at = 0
since = 0
last_read = 0
was_pressed = False

while True:
    now = time.ticks_ms()
    if time.ticks_diff(now, last_read) >= READ_MS:
        last_read = now
        pressed = button.value() == 1
        if pressed and not was_pressed and pattern is None:
            print("press")
            pattern, at, since = DOUBLE_MS, 0, now
            motor.on()             # every pattern starts on
        was_pressed = pressed
    if pattern and time.ticks_diff(now, since) >= pattern[at]:
        at += 1
        since = now
        if at == len(pattern):     # the end of the list
            pattern = None
            motor.off()
        else:
            motor.value(1 if at % 2 == 0 else 0)   # even: on
