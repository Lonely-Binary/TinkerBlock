"""
  Rotary Potentiometer - calibrating, MicroPython     TK08 / /p/tk08

  Wiring. Count from the square pad on the TinkerBlock board, knob
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: the full turn puts VCC on your pin)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Answer in the shell: press Enter at each mark.
    Nothing to install: machine and time are built in.
"""

from machine import ADC, Pin
import sys
import time

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
POT_PIN = 4

pot = ADC(Pin(POT_PIN))
if sys.platform == "esp32":
    pot.atten(ADC.ATTN_11DB)        # widest range, roughly 0 to 3.1 V

MARK_PCT = [0, 25, 50, 75, 100]


def read_steady():                  # sixteen readings, averaged
    total = 0
    for _ in range(16):
        total += pot.read_u16() >> 4    # 0 to 4095
    return total // 16


def position_of(reading, table):
    if reading <= table[0]:
        return MARK_PCT[0]
    for i in range(1, len(table)):
        if reading <= table[i]:
            span = table[i] - table[i - 1]
            f = (reading - table[i - 1]) / span if span > 0 else 0
            return MARK_PCT[i - 1] + f * (MARK_PCT[i] - MARK_PCT[i - 1])
    return MARK_PCT[-1]


table = []
for pct in MARK_PCT:
    input("Turn the knob to {} % and press Enter.".format(pct))
    table.append(read_steady())
    print("  reads", table[-1])

while True:
    reading = read_steady()
    print(reading, " -> ", "{:.1f} %".format(position_of(reading, table)))
    time.sleep_ms(200)
