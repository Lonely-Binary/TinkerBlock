from machine import Pin, PWM
import time

# ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
red = PWM(Pin(4), freq=1000)
green = PWM(Pin(5), freq=1000)
blue = PWM(Pin(6), freq=1000)

def set_colour(r, g, b):
    # 0 is off, 255 is full. Common cathode: no "255 -" anywhere.
    red.duty_u16(r * 257)
    green.duty_u16(g * 257)
    blue.duty_u16(b * 257)

while True:
    set_colour(255, 0, 0)    # red
    time.sleep(1)
    set_colour(0, 255, 0)    # green
    time.sleep(1)
    set_colour(0, 0, 255)    # blue
    time.sleep(1)
