from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
button = Pin(4, Pin.IN)       # no pull: the block has its own

while True:
    if button.value() == 1:   # 1 is pressed
        print("pressed")
    else:
        print("released")
    time.sleep_ms(200)
