from machine import Pin

# CLOCK and DATA. ESP32: 25, 26. ESP32-S3: 4, 5. Pico: 13, 14.
clock = Pin(4, Pin.IN)        # no pull: the block has its own
data = Pin(5, Pin.IN)
count = 0
last_clock = 1

while True:
    level = clock.value()
    if level != last_clock:
        if level == 0:        # CLOCK has just fallen
            count += 1 if data.value() == 1 else -1
            print(count)
        last_clock = level
