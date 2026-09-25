"""
  Touch Sensor - first read, MicroPython              TK43 / /p/tk43

  Wiring. Count from the square pad on the TinkerBlock board, pad
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: touched, SIGNAL gives your pin VCC)
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

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
TOUCH_PIN = 4

touch = Pin(TOUCH_PIN, Pin.IN)      # the chip drives SIGNAL both ways
last = 0

while True:
    reading = touch.value()
    if reading != last:             # print only when it changes
        print("HIGH  touched" if reading else "LOW   released")
        last = reading
