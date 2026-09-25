from machine import Pin
import time

# ESP32: 25 26 27 32. ESP32-S3: 4 5 6 7. Pico: 10 11 12 13.
a, b, c, d = [Pin(n, Pin.IN, Pin.PULL_UP) for n in (4, 5, 6, 7)]

def release_all():              # every line an input with its pull-up
    for p in (a, b, c, d):
        p.init(Pin.IN, Pin.PULL_UP)

def scan_tilt():
    release_all()
    a.init(Pin.OUT, value=0)    # hold A LOW, read B and D
    time.sleep_us(10)
    ab, da = b.value() == 0, d.value() == 0
    release_all()
    c.init(Pin.OUT, value=0)    # hold C LOW, read B and D
    time.sleep_us(10)
    bc, cd = b.value() == 0, d.value() == 0
    release_all()
    if ab: return "left"
    if da: return "header"
    if bc: return "top"
    if cd: return "right"
    return "none"

while True:
    print(scan_tilt())
    time.sleep_ms(200)
