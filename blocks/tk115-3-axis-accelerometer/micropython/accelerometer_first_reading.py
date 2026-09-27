"""
  3-Axis Accelerometer - first reading, MicroPython     TK115 / /p/tk115

  Wiring. Count from the square pad, which is GND. Chip side up,
  header at the bottom, left to right:

    GND -> GND
    3V3 -> 3V3. 3.3 V only: the chip's limit is 3.6 V.
    SCL -> GPIO 9 on an ESP32-S3, GPIO 22 on an ESP32, GP5 on a Pico
    SDA -> GPIO 8 on an ESP32-S3, GPIO 21 on an ESP32, GP4 on a Pico

  Change SDA_PIN and SCL_PIN below for an ESP32 or a Pico.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    No library needed: the registers are all in this file.
"""

from machine import I2C, Pin
import time

# 0x19: the board leaves the chip's SDO pin open and the chip pulls it
# high itself.
ADDR = 0x19
# GPIO numbers. ESP32-S3: 8 and 9. ESP32: 21 and 22. Pico: 4 and 5.
SDA_PIN = 8
SCL_PIN = 9
i2c = I2C(0, sda=Pin(SDA_PIN), scl=Pin(SCL_PIN), freq=100_000)

try:
    found = i2c.readfrom_mem(ADDR, 0x0F, 1)[0] == 0x11   # WHO_AM_I
except OSError:
    found = False
if not found:
    print("no SC7A20 at 0x19: check GND, then SDA and SCL")
    raise SystemExit

# It powers up asleep. CTRL_REG1: 100 readings a second, X Y Z on.
i2c.writeto_mem(ADDR, 0x20, bytes([0x57]))
# CTRL_REG4: +-2 g, and never half of one reading and half the next.
i2c.writeto_mem(ADDR, 0x23, bytes([0x80]))


def axis(lo, hi):
    v = (hi << 8) | lo
    if v & 0x8000:
        v -= 0x10000                  # two's complement
    return (v >> 4) / 1000            # 12 bits at the top; 1 mg a count


def read_g():
    # 0x28 is OUT_X_L; bit 7 set steps through all six registers.
    b = i2c.readfrom_mem(ADDR, 0x28 | 0x80, 6)
    return axis(b[0], b[1]), axis(b[2], b[3]), axis(b[4], b[5])


print("SC7A20 found, awake at 100 Hz")
while True:
    x, y, z = read_g()
    print("X %6.3f  Y %6.3f  Z %6.3f  g" % (x, y, z))
    time.sleep_ms(200)
