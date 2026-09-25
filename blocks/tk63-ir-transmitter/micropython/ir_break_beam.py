"""
  Infrared Transmitter - a beam you can break     TK63 + TK64 / /p/tk63

  Wiring. Count from the square pad on each TinkerBlock board, parts
  up, header at the bottom:

    TK63 transmitter
      GND    -> GND
      VCC    -> 5V, or VBUS on a Pico (only feeds the LEDs)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> GPIO 22, GPIO 6 or GP17

    TK64 receiver
      GND    -> GND
      VCC    -> 3V3
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> GPIO 34, GPIO 4 or GP26

  ESP32, ESP32-S3, Pico, in that order. Stand the two blocks face
  to face and keep the beam clear while it starts.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import ADC, Pin

# TK63: ESP32 22, ESP32-S3 6, Pico 17.
IR_PIN = 6
# TK64: ESP32 34, ESP32-S3 4, Pico 26.
SENSE_PIN = 4

ir = Pin(IR_PIN, Pin.OUT, value=0)   # LOW: LEDs off
adc = ADC(Pin(SENSE_PIN))
try:
    adc.atten(ADC.ATTN_11DB)         # ESP32 and S3: read to about 3.1 V
except AttributeError:
    pass                             # the Pico has no attenuator

def beam():
    # Off, then on: the difference is the TK63's light alone.
    ir.value(0)
    time.sleep_ms(2)
    dark = adc.read_u16()
    ir.value(1)
    time.sleep_ms(2)
    lit = adc.read_u16()
    ir.value(0)
    return lit - dark

time.sleep_ms(500)
clear = sum(beam() for _ in range(16)) // 16
print("clear beam:", clear)

broken = False
while True:
    now = beam() < clear // 2        # under half the clear beam
    if now != broken:
        broken = now
        print("beam broken" if broken else "beam clear")
    time.sleep_ms(20)
