"""
  Logic Level Converter - I2C scan, MicroPython         TK97 / /p/tk97

  A 3.3 V board and a 5 V I2C part through the TK97. Socket at the
  top, both rows start at GND at the left. The board goes on the OUT
  side, the part on the IN side:

    GND    -> GND
    5V     -> 5V (VBUS on a Pico), and the part's VCC to 5V too.
    3V3    -> nothing. It is the TK97's output, never your 3V3.
    L1     -> SDA: GPIO 21 on an ESP32, GPIO 8 on an S3, GP4
    L2     -> SCL: GPIO 22 on an ESP32, GPIO 9 on an S3, GP5
    H1, H2 -> the part's SDA and SCL.

  The TK97's 10 kOhm pull-ups are the bus's pull-ups on both sides.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import I2C, Pin

# GPIO numbers. ESP32: 21 and 22. ESP32-S3: 8 and 9. Pico: 4 and 5.
SDA_PIN = 8
SCL_PIN = 9

i2c = I2C(0, sda=Pin(SDA_PIN), scl=Pin(SCL_PIN), freq=100000)

while True:
    found = i2c.scan()
    if not found:
        print("nothing answered: 5V, GND, then SDA and SCL")
    for addr in found:
        print("found 0x%02X" % addr)
    time.sleep(3)
