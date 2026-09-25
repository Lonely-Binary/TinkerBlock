from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
knock = Pin(4, Pin.IN)        # no pull: the block has its own
HOLD_OFF_MS = 100
last = 0
last_knock = time.ticks_add(time.ticks_ms(), -HOLD_OFF_MS)
knocks = 0

while True:
    level = knock.value()
    now = time.ticks_ms()
    if level == 1 and last == 0:
        if time.ticks_diff(now, last_knock) >= HOLD_OFF_MS:
            knocks += 1
            last_knock = now
            print("knocks:", knocks)
    last = level
