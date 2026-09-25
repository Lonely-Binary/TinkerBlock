"""
  EEPROM Memory - boot counter, MicroPython           TK31 / /p/tk31

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
    Save it to the board as main.py to count real power-ups.
    Nothing to install: machine and time are built in.
"""

import time
from machine import I2C, Pin

# SDA, SCL. ESP32: 21, 22. ESP32-S3: 8, 9. Pico: 4, 5.
i2c = I2C(0, sda=Pin(21), scl=Pin(22), freq=100_000)

EEPROM = 0x50                 # fixed: A0 to A2 are tied to GND
COUNT_AT = 0                  # the byte that holds the count

count = i2c.readfrom_mem(EEPROM, COUNT_AT, 1, addrsize=16)[0]
if count == 255:              # a byte never written
    count = 0
count += 1
i2c.writeto_mem(EEPROM, COUNT_AT, bytes([count]), addrsize=16)
time.sleep_ms(5)              # up to 5 ms, answering nobody

print("Started", count, "times. Unplug it, plug it back in.")
