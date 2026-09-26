"""
  SHT31 - first reading, MicroPython                    TK120 / /p/tk120

  Wiring. Count from the square pad on the TinkerBlock board, sensor
  side up, header at the bottom:

    GND -> GND
    VCC -> 3V3. Never 5V: the board's pull-ups put VCC on SDA and SCL.
    SDA -> GPIO 8 on an ESP32-S3, GPIO 21 on an ESP32, GP4 on a Pico
    SCL -> GPIO 9 on an ESP32-S3, GPIO 22 on an ESP32, GP5 on a Pico

  Change SDA_PIN and SCL_PIN below for an ESP32 or a Pico.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    No library needed: the command, the checksum and the arithmetic
    are all in this file.
"""

from machine import I2C, Pin
import time

# 0x2400: measure once, high repeatability, no clock stretching.
ADDR = 0x44
# GPIO numbers. ESP32-S3: 8 and 9. ESP32: 21 and 22. Pico: 4 and 5.
SDA_PIN = 8
SCL_PIN = 9
i2c = I2C(0, sda=Pin(SDA_PIN), scl=Pin(SCL_PIN), freq=100_000)

def crc8(data):
    crc = 0xFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 0x80:
                crc = ((crc << 1) ^ 0x31) & 0xFF
            else:
                crc = (crc << 1) & 0xFF
    return crc

def read():
    i2c.writeto(ADDR, b"\x24\x00")
    time.sleep_ms(20)                 # 15 ms is the datasheet maximum
    d = i2c.readfrom(ADDR, 6)         # temperature first, then humidity
    if d[2] != crc8(d[0:2]) or d[5] != crc8(d[3:5]):
        return None                   # a byte did not survive the wires
    t = -45 + 175 * ((d[0] << 8) | d[1]) / 65535
    rh = 100 * ((d[3] << 8) | d[4]) / 65535
    return t, rh

while True:
    r = read()
    print("%.2f C  %.2f %%" % r if r else "read failed")
    time.sleep(2)
