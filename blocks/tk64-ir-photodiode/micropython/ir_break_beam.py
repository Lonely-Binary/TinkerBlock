"""
  Infrared Receiver - a beam that breaks, MicroPython  TK64 / /p/tk64

  A TK63 Infrared Transmitter sends the beam; this TK64 reads it.
  Stand them face to face, a few centimetres apart to start.

  Wiring, TK64. Count from the square pad on the TinkerBlock board,
  parts up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: strong infrared takes SIGNAL to
              about VCC - 0.4 V)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 34 on an ESP32,
              GP26 on a Raspberry Pi Pico

  Wiring, TK63: GND to GND, VCC to 3V3, NC to nothing, and its
  SIGNAL to GPIO 6 on an ESP32-S3, GPIO 22 on an ESP32, GP17 on a
  Pico.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# TK64 SIGNAL, an ADC pin. ESP32: 34. ESP32-S3: 4. Pico: 26.
IR_PIN = 4
# TK63 SIGNAL, HIGH for the beam. ESP32: 22. ESP32-S3: 6. Pico: 17.
TX_PIN = 6

SETTLE_MS = 2                         # let the sensor settle

adc = ADC(Pin(IR_PIN))
if sys.platform == "esp32":           # ESP32 and ESP32-S3
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V
tx = Pin(TX_PIN, Pin.OUT, value=0)


def read_ir():
    return sum(adc.read_u16() for _ in range(8)) // 8


def beam():
    """One round: the room alone, then the room and the beam."""
    tx.value(0)
    time.sleep_ms(SETTLE_MS)
    off = read_ir()                   # the room's own infrared
    tx.value(1)
    time.sleep_ms(SETTLE_MS)
    on = read_ir()                    # the room and the beam
    tx.value(0)
    return on - off                   # the beam alone


time.sleep_ms(500)
clear = sum(beam() for _ in range(20)) // 20   # nothing in the way
print("clear beam:", clear)
if clear < 300:
    print("weak beam: move closer, aim, or shade it")

was_broken = False
while True:
    broken = beam() < clear // 2      # under half: something is there
    if broken != was_broken:
        print("beam broken" if broken else "beam clear")
        was_broken = broken
    time.sleep_ms(10)
