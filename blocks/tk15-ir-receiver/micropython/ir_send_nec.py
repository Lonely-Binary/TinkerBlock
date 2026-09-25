"""
  IR Sender - send a NEC code, MicroPython            TK16 / /p/tk16

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V for full range (VBUS on a Pico); 3V3 works, shorter
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 22 on an ESP32, GPIO 6 on an ESP32-S3,
              GP17 on a Raspberry Pi Pico

  Install once, from a computer:
    mpremote mip install "github:peterhinch/micropython_ir/ir_tx"

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import Pin
from ir_tx.nec import NEC

# The GPIO number SIGNAL is wired to. ESP32: 22. ESP32-S3: 6. Pico: 17.
IR_TX_PIN = 6

# REPLACE THESE with what the key-code sketch printed for your remote.
ADDRESS = 0x04
COMMAND = 0x16

nec = NEC(Pin(IR_TX_PIN, Pin.OUT, value=0))   # LOW: LED off
while True:
    print("sending")
    nec.transmit(ADDRESS, COMMAND)
    time.sleep(2)
