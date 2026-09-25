"""
  TM1637 4-Digit Display - the first number        TK51 / /p/tk51

  Wiring. No pad on this board is square, so go by the names
  printed on the back: GND, VCC, clock, data. Seen from the
  display side with the header on the left, GND is the top pin.

    GND   -> GND
    VCC   -> 3V3 on an ESP32 or ESP32-S3, 3V3(OUT) on a Pico.
             Never 5V: the pull-ups would put 5 V on your pins.
    CLOCK -> GPIO 18 on an ESP32, GPIO 4 on an ESP32-S3,
             GP2 on a Raspberry Pi Pico
    DATA  -> GPIO 19 on an ESP32, GPIO 5 on an ESP32-S3,
             GP3 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save tm1637.py from this folder to the board first.
"""

import time
from tm1637 import TM1637

# CLOCK, DATA. ESP32: 18, 19. ESP32-S3: 4, 5. Pico: 2, 3.
display = TM1637(18, 19)

display.brightness(4)       # sent with the next write
display.number(1234)        # stays there with no further help
time.sleep(2)

while True:
    for i in range(61):
        display.number(i)   # leading zeros off: 7 is "   7"
        time.sleep_ms(500)
