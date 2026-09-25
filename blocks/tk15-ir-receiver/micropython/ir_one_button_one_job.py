"""
  IR Receiver - one button, one job, MicroPython      TK15 / /p/tk15

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (SIGNAL idles at VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 23 on an ESP32, GPIO 9 on an ESP32-S3,
              GP16 on a Raspberry Pi Pico

  The LED: a TK01 XL LED (SIGNAL to LED_PIN, GND to GND), or any LED
  with a resistor, on GPIO 4, GPIO 5 or GP15.

  Install once, from a computer:
    mpremote mip install "github:peterhinch/micropython_ir/ir_rx"

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import Pin, PWM
from ir_rx.nec import NEC_8

# The GPIO number SIGNAL is wired to. ESP32: 23. ESP32-S3: 9. Pico: 16.
IR_RX_PIN = 9
# A PWM pin for the LED. ESP32: 4. ESP32-S3: 5. Pico: 15.
LED_PIN = 5

# REPLACE THESE with commands your own remote sent.
CMD_POWER = 0x16                 # toggles: repeats ignored
CMD_UP = 0x17                    # dims up: every repeat counts
CMD_DOWN = 0x18                  # dims down: every repeat counts

led = PWM(Pin(LED_PIN), freq=1000)
on = False
level = 128                      # 16 to 255
last = None                      # the command a repeat repeats

def show():
    led.duty_u16(level * 257 if on else 0)
    print("on " if on else "off", level)

def got(command, address, ctrl):
    global on, level, last
    repeat = command < 0         # a repeat frame carries no data
    if repeat:
        command = last
    else:
        last = command
    if command == CMD_POWER and not repeat:
        on = not on              # once per press
    elif command == CMD_UP and on:
        level = min(level + 16, 255)
    elif command == CMD_DOWN and on:
        level = max(level - 16, 16)
    else:
        return
    show()

ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), got)
show()
while True:
    time.sleep_ms(500)
