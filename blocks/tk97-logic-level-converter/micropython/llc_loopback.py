"""
  Logic Level Converter - loopback check, MicroPython   TK97 / /p/tk97

  Two channels, both ways, with one jumper and no other part. Socket
  at the top, both rows start at GND at the left. The board goes on
  the OUT side:

    GND    -> GND
    5V     -> 5V (VBUS on a Pico). The TK97 makes its own 3.3 V.
    3V3    -> nothing. It is the TK97's output, never your 3V3.
    L1     -> OUT_PIN: GPIO 25 on an ESP32, GPIO 4 on an S3, GP14
    L2     -> BACK_PIN: GPIO 26 on an ESP32, GPIO 5 on an S3, GP15
    H1, H2    joined to each other with one jumper.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import Pin

# GPIO numbers. ESP32: 25 and 26. ESP32-S3: 4 and 5. Pico: 14 and 15.
OUT_PIN = 4
BACK_PIN = 5

out = Pin(OUT_PIN, Pin.OUT, value=0)
# No pull-up here: the TK97's own 10 kOhm holds the line.
back = Pin(BACK_PIN, Pin.IN)


def send_and_read(level):
    out.value(level)
    time.sleep_ms(1)
    return back.value()


while True:
    high = send_and_read(1)
    low = send_and_read(0)
    if high == 1 and low == 0:
        print("HIGH came back HIGH, LOW came back LOW: ok")
    elif high == 1 and low == 1:
        print("always HIGH: check the jumper, and BACK_PIN")
    elif high == 0 and low == 0:
        print("always LOW: check 5V and GND, then OUT_PIN")
    else:
        print("backwards: check every wire against the list")
    time.sleep(1)
