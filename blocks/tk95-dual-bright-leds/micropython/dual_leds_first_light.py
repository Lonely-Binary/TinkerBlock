"""
  Dual Bright LEDs - the first light, MicroPython      TK95 / /p/tk95

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V (VBUS on a Pico). It only feeds the LEDs.
    3000K  -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3, GP14 on a Pico
    6500K  -> GPIO 26 on an ESP32, GPIO 5 on an ESP32-S3, GP15 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)

  Do not look into the LEDs from close up.
"""

import time
from machine import Pin, PWM

# GPIO numbers. ESP32: 25 and 26. ESP32-S3: 4 and 5. Pico: 14 and 15.
WARM_PIN = 4
COOL_PIN = 5

# Two thirds of full: with VCC on 5V an LED held fully on runs past
# its rating. With VCC on 3V3, 65535 is fine.
MAX_DUTY = 43690

# LOW first: nothing on the board holds the transistors off.
Pin(WARM_PIN, Pin.OUT, value=0)
Pin(COOL_PIN, Pin.OUT, value=0)
warm = PWM(Pin(WARM_PIN))
cool = PWM(Pin(COOL_PIN))
for led in (warm, cool):
    led.freq(1000)
    led.duty_u16(0)


def show(w, c, what):
    warm.duty_u16(w)
    cool.duty_u16(c)
    print(what)
    time.sleep(2)


while True:
    show(MAX_DUTY, 0, "warm: 3000K")
    show(0, MAX_DUTY, "cool: 6500K")
    show(MAX_DUTY // 2, MAX_DUTY // 2, "both, half each: about 4100 K")
    show(0, 0, "off")
