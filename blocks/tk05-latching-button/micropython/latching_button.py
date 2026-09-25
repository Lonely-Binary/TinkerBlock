from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
switch = Pin(4, Pin.IN)   # no pull: the board's pull-down holds it LOW

while True:
    print("ON" if switch.value() else "OFF")
    time.sleep(0.5)
