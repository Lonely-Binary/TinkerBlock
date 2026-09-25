from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
hall = Pin(4, Pin.IN)         # the chip drives SIGNAL both ways

while True:
    if hall.value() == 0:     # active low: 0 is a magnet
        print("magnet")
    else:
        print("no magnet")
    time.sleep_ms(200)
