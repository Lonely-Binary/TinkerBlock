"""
  IR loop-back test, MicroPython          TK15 + TK16 / /p/tbir

  Wiring. Count from the square pad on each TinkerBlock board, parts
  up, header at the bottom:

    TK15 receiver
      GND    -> GND
      VCC    -> 3V3   (SIGNAL idles at VCC)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> GPIO 23, GPIO 9 or GP16

    TK16 sender
      GND    -> GND
      VCC    -> 5V, or VBUS on a Pico (only feeds the LEDs)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> GPIO 22, GPIO 6 or GP17

  ESP32, ESP32-S3, Pico, in that order.

  Install once, from a computer:
    mpremote mip install "github:peterhinch/micropython_ir/ir_rx"
    mpremote mip install "github:peterhinch/micropython_ir/ir_tx"

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import Pin
from ir_rx.nec import NEC_8
from ir_tx.nec import NEC

# The GPIO numbers. ESP32: 23. ESP32-S3: 9. Pico: 16.
IR_RX_PIN = 9
# ESP32: 22. ESP32-S3: 6. Pico: 17.
IR_TX_PIN = 6

ADDRESS = 0x12                   # any pair nothing else is sending
COMMAND = 0x34

sent = 0
back = 0

def got(command, address, ctrl):
    global back
    if command == COMMAND and address == ADDRESS:
        back += 1

ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), got)
nec = NEC(Pin(IR_TX_PIN, Pin.OUT, value=0))
print("Loop-back: sending once a second.")
while True:
    nec.transmit(ADDRESS, COMMAND)
    sent += 1
    time.sleep(1)
    print("ok" if back == sent else "missing", back, "of", sent)
