"""
  Infrared Speed Sensor - first read, MicroPython     TK61 / /p/tk61

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (the block pulls SIGNAL up to VCC with 10k)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine is built in.
"""

from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
SENSOR_PIN = 4

sensor = Pin(SENSOR_PIN, Pin.IN)    # the board drives SIGNAL both ways
last = -1
passes = 0

while True:
    level = sensor.value()
    if level != last:               # print only when it changes
        if level:
            passes += 1
            print("blocked  (pass", passes, ")")
        else:
            print("clear")
        last = level
    time.sleep_ms(10)
