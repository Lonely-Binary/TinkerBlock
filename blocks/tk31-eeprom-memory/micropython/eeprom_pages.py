"""
  EEPROM Memory - text across a page edge, MicroPython  TK31 / /p/tk31

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
PAGE = 64                     # the chip's page
PIECE = 16
START = 50                    # crosses the edge at 64 on purpose
TEXT = b"Written across a page edge, read back whole."

def wait_ready():
    t0 = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), t0) < 20:
        try:
            i2c.writeto(EEPROM, b"\x00\x00")
            return
        except OSError:       # still writing
            pass

def write_block(at, data):
    while data:
        room = PAGE - at % PAGE           # bytes left in this page
        n = min(len(data), room, PIECE)
        i2c.writeto_mem(EEPROM, at, data[:n], addrsize=16)
        wait_ready()                      # one wait per piece
        at += n
        data = data[n:]

write_block(START, TEXT)
back = i2c.readfrom_mem(EEPROM, START, len(TEXT), addrsize=16)
print("wrote:", TEXT.decode())
print("read: ", back.decode())
