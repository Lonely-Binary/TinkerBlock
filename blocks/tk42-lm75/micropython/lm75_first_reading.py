"""
  LM75 Temperature Sensor - first reading         TK42 / /p/tk42

  Wiring. Count from the square pad on the TinkerBlock board,
  parts up, header at the bottom:

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
    Nothing to install: machine, struct and time are built in.
"""

import struct
import time
from machine import I2C, Pin

# SDA, SCL. ESP32: 21, 22. ESP32-S3: 8, 9. Pico: 4, 5.
i2c = I2C(0, sda=Pin(21), scl=Pin(22), freq=100_000)

LM75 = 0x48                 # fixed: A0 to A2 are tied to GND

def celsius():
    two = i2c.readfrom_mem(LM75, 0x00, 2)  # register 0
    return struct.unpack(">h", two)[0] / 256

time.sleep_ms(250)
celsius()                   # the first one is thrown away
while True:
    print("{:.3f} C".format(celsius()))
    time.sleep_ms(500)
