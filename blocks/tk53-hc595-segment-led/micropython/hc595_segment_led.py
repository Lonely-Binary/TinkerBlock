"""
  74HC595 Segment LED - count 0 to 9, MicroPython       TK53 / /p/tk53

  Wiring. Count from pin 1, printed GND-1 on the TinkerBlock board
  (this board has no square pad), digit up, header at the bottom:

    GND   -> GND
    VCC   -> 3V3   (never 5V: at 5V the chip may not read 3.3 V
             as HIGH)
    NC    -> nothing   (unconnected on the board)
    LATCH -> GPIO 27 on an ESP32, GPIO 6 on an ESP32-S3, GP2 on a Pico
    CLOCK -> GPIO 26 on an ESP32, GPIO 5 on an ESP32-S3, GP3 on a Pico
    DATA  -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3, GP4 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine is built in.
"""

from machine import Pin
import time

# The GPIO numbers LATCH, CLOCK and DATA are wired to.
# ESP32: 27, 26, 25. ESP32-S3: 6, 5, 4. Pico: 2, 3, 4.
latch = Pin(6, Pin.OUT)
clock = Pin(5, Pin.OUT)
data = Pin(4, Pin.OUT)

# One byte per digit. Bit 0 is segment a (QA) and bit 7 the
# decimal point (QH). The digit is common cathode: a 1 lights.
DIGITS = [0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F]
DOT = 0x80


def show(pattern):
    """Eight bits in, most significant first, then one latch."""
    latch.value(0)
    for i in range(7, -1, -1):
        data.value((pattern >> i) & 1)
        clock.value(1)              # the rising edge shifts it in
        clock.value(0)
    latch.value(1)                  # the rising edge shows it


show(0)                             # blank
while True:
    for n in range(10):
        show(DIGITS[n])
        print(n, hex(DIGITS[n]))
        time.sleep(1)
    show(DOT)                       # a dot between rounds
    time.sleep(1)
