"""
  TM1637 4-Digit Display - a voltmeter readout     TK51 / /p/tk51

  The voltage on GPIO 34 of an ESP32, to two decimals, on a
  digit panel.

  Wiring. No pad on this board is square, so go by the names
  printed on the back: GND, VCC, clock, data. Seen from the
  display side with the header on the left, GND is the top pin.

    GND   -> GND
    VCC   -> 3V3. Never 5V: the pull-ups would put 5 V on your
             pins.
    CLOCK -> GPIO 18
    DATA  -> GPIO 19
  And the thing being measured, 0 to 3.3 V only:
    a potentiometer's outer pins to 3V3 and GND, its wiper to
    GPIO 34.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32)
    Save tm1637.py from this folder to the board first.
"""

import time
from machine import ADC, Pin
from tm1637 import TM1637

POINT = 0b10000000          # after the first digit written

display = TM1637(18, 19)
display.brightness(4)
display.segments([0])       # position 0 is never written below

adc = ADC(Pin(34))
adc.atten(ADC.ATTN_11DB)    # the whole 0 to 3.3 V range

while True:
    mv = adc.read_uv() // 1000          # calibrated millivolts
    centivolts = (mv + 5) // 10         # 0 to 330
    # Three digits from position 1, leading zeros on, the point
    # after the first of them: 0.05 and 3.21 both read right.
    display.number(centivolts, POINT, True, 3, 1)
    time.sleep_ms(200)
