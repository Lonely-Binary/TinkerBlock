"""
  EEPROM Memory - save a setting, MicroPython         TK31 / /p/tk31

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
    Type numbers into the shell at the bottom.
    Nothing to install: machine and time are built in.
"""

import time
from machine import I2C, Pin

# SDA, SCL. ESP32: 21, 22. ESP32-S3: 8, 9. Pico: 4, 5.
i2c = I2C(0, sda=Pin(21), scl=Pin(22), freq=100_000)

EEPROM = 0x50
SETTING_AT = 16               # any address; move it if it wears
writes = 0

def read_byte(at):
    return i2c.readfrom_mem(EEPROM, at, 1, addrsize=16)[0]

def update(at, value):
    global writes
    if read_byte(at) == value:        # same: no write, no wear
        return False
    i2c.writeto_mem(EEPROM, at, bytes([value]), addrsize=16)
    time.sleep_ms(5)
    writes += 1
    return True

print("saved setting:", read_byte(SETTING_AT))
while True:
    v = int(input("a number from 0 to 255: "))
    if 0 <= v <= 255:
        wrote = update(SETTING_AT, v)
        print(v, "saved" if wrote else "unchanged, not written",
              "(writes this run:", writes, ")")
