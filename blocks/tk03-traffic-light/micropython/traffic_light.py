from machine import Pin
import time

# GPIO numbers. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
red = Pin(4, Pin.OUT, value=0)
yellow = Pin(5, Pin.OUT, value=0)
green = Pin(6, Pin.OUT, value=0)


def show(r, y, g):
    red.value(r)
    yellow.value(y)
    green.value(g)


while True:
    show(1, 0, 0)   # red
    time.sleep(5)
    show(0, 0, 1)   # green
    time.sleep(5)
    show(0, 1, 0)   # yellow
    time.sleep(2)
