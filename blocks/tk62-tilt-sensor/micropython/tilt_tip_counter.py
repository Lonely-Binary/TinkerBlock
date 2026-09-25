"""
  Tilt Sensor - counting tips, MicroPython              TK62 / /p/tk62

  Wiring, the same as the first read. Count from the square pad on
  the TinkerBlock board, parts up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 25 on an ESP32,
              GP15 on a Raspberry Pi Pico

  Mounting: lay the block along the thing that tips, with the
  board's left and right edges toward its two ends.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
"""

from machine import Pin
import time

# ESP32: 25. ESP32-S3: 4. Pico: 15.
TILT_PIN = 4

# A change counts once it has held this long: longer than the
# balls rattle, shorter than a real tip.
STABLE_MS = 50

tilt = Pin(TILT_PIN, Pin.IN)    # the board has its own pull-down

raw = tilt.value()
steady = raw
raw_since = time.ticks_ms()
tips = 0
print("left end down" if steady else "right end down")

while True:
    level = tilt.value()
    if level != raw:            # a rattle, or a real tip starting
        raw = level
        raw_since = time.ticks_ms()

    # Believe a change only once it has held for STABLE_MS.
    held = time.ticks_diff(time.ticks_ms(), raw_since)
    if raw != steady and held >= STABLE_MS:
        steady = raw
        tips += 1
        side = "left" if steady else "right"
        print(f"tip {tips}: {side} end down")
