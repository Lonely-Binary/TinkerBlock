"""
  Infrared Transmitter - to a TK15, MicroPython  TK63 + TK15 / /p/tk63

  Wiring. Count from the square pad on each TinkerBlock board, parts
  up, header at the bottom:

    TK63 transmitter
      GND    -> GND
      VCC    -> 5V, or VBUS on a Pico (only feeds the LEDs)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> GPIO 22, GPIO 6 or GP17

    TK15 receiver
      GND    -> GND
      VCC    -> 3V3   (its SIGNAL idles at VCC)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> GPIO 23, GPIO 9 or GP16

  ESP32, ESP32-S3, Pico, in that order. Stand the two blocks face
  to face, about 30 cm apart.

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

# The GPIO numbers. TK63: ESP32 22, ESP32-S3 6, Pico 17.
IR_PIN = 6
# TK15: ESP32 23, ESP32-S3 9, Pico 16.
RX_PIN = 9

ADDRESS = 0x12                   # any address nothing else is sending
heard = []

def got(command, address, ctrl):
    if address == ADDRESS:
        heard.append(command)

rx = NEC_8(Pin(RX_PIN, Pin.IN), got)
tx = NEC(Pin(IR_PIN, Pin.OUT, value=0))   # LOW: LEDs off

command = 0
while True:
    heard.clear()
    tx.transmit(ADDRESS, command)   # returns at once; the frame follows
    time.sleep_ms(200)
    if command in heard:
        print("sent", command, " heard it")
    else:
        print("sent", command, " nothing heard")
    command = (command + 1) % 256
    time.sleep(1)
