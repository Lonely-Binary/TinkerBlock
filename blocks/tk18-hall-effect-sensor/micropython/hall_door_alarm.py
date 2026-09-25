"""
  Hall Effect Sensor - a door alarm, MicroPython      TK18 / /p/tk18

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: with no magnet, SIGNAL sits at VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  The block on the frame, a magnet on the door facing the small
  chip at the top left of the block.

  A TK01 XL LED on a second pin is the alarm (optional):
    GND    -> GND
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3, GP14 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO number the Hall sensor's SIGNAL is wired to.
# ESP32: 25. ESP32-S3: 4. Pico: 15.
HALL_PIN = 4
# The GPIO number the TK01's SIGNAL is wired to.
# ESP32: 4. ESP32-S3: 5. Pico: 14.
LED_PIN = 5
ALARM_AFTER_MS = 10000

hall = Pin(HALL_PIN, Pin.IN)        # the chip drives SIGNAL both ways
led = Pin(LED_PIN, Pin.OUT, value=0)

def door_shut():
    return hall.value() == 0        # magnet near: 0

was_shut = door_shut()
opened_at = time.ticks_ms()
alarm_on = False

while True:
    shut = door_shut()
    now = time.ticks_ms()

    if not shut and was_shut:       # it has just opened
        opened_at = now
        print("opened")
    if shut and not was_shut:
        print("shut")
    was_shut = shut

    alarm = (not shut
             and time.ticks_diff(now, opened_at) >= ALARM_AFTER_MS)
    if alarm != alarm_on:
        alarm_on = alarm
        led.value(alarm)
        print("left open: alarm" if alarm else "alarm off")
