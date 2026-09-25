"""
  IR Receiver - key codes, MicroPython                TK15 / /p/tk15

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (SIGNAL idles at VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 23 on an ESP32, GPIO 9 on an ESP32-S3,
              GP16 on a Raspberry Pi Pico

  Install once, from a computer:
    mpremote mip install "github:peterhinch/micropython_ir/ir_rx"

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
"""

import time
from machine import Pin
from ir_rx.nec import NEC_8     # NEC, 8-bit address and its check

# The GPIO number SIGNAL is wired to. ESP32: 23. ESP32-S3: 9. Pico: 16.
IR_RX_PIN = 9

seen = set()

def got(command, address, ctrl):
    if command < 0:             # a repeat frame carries no data
        print("[repeat]")
        return
    key = (address, command)
    first = key not in seen
    seen.add(key)
    line = "address=0x%02x  command=0x%02x" % (address, command)
    if first:
        line += "  first time, %d so far" % len(seen)
    print(line)

ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), got)
print("Press every button on the remote.")
while True:
    time.sleep_ms(500)
