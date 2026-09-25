from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
bumper = Pin(4, Pin.IN)       # no pull: the block has its own

while True:
    if bumper.value() == 1:   # 1 is a hit
        print("hit")
    else:
        print("clear")
    time.sleep_ms(200)
