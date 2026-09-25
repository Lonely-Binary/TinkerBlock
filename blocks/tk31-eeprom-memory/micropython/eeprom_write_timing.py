"""
  EEPROM Memory - how long a write takes, MicroPython   TK31 / /p/tk31

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND -> GND
    VCC -> 3V3 on an ESP32 or ESP32-S3, 3V3(OUT) on a Pico.
           Never 5V: the pull-ups would put 5 V on your pins.
    SDA -> GPIO 21 on an ESP32, GPIO 8 on an ESP32-S3,
           GP4 on a Raspberry Pi Pico
    SCL -> GPIO 22 on an ESP32, GPIO 9 on an ESP32-S3,
           GP5 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Nothing to install: machine and time are built in.
"""

import time
from machine import I2C, Pin

# SDA, SCL. ESP32: 21, 22. ESP32-S3: 8, 9. Pico: 4, 5.
i2c = I2C(0, sda=Pin(21), scl=Pin(22), freq=100_000)
EEPROM = 0x50

def answers():
    try:
        i2c.writeto(EEPROM, b"\x00\x00")   # set the pointer only
        return True
    except OSError:                        # no acknowledge
        return False

def wait_ready():
    t0 = time.ticks_us()
    while not answers():
        if time.ticks_diff(time.ticks_us(), t0) > 20_000:
            break
    return time.ticks_diff(time.ticks_us(), t0)

i2c.writeto_mem(EEPROM, 100, b"\x2a", addrsize=16)
print("asked straight away:", "answered" if answers() else "no answer")
wait_ready()

for i in range(5):
    i2c.writeto_mem(EEPROM, 100 + i, bytes([i]), addrsize=16)
    print("write", i, "was done after", wait_ready(), "us")
