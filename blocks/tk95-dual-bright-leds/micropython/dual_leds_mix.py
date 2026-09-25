"""
  Dual Bright LEDs - warm to cool at one brightness    TK95 / /p/tk95

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V (VBUS on a Pico). It only feeds the LEDs.
    3000K  -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3, GP14 on a Pico
    6500K  -> GPIO 26 on an ESP32, GPIO 5 on an ESP32-S3, GP15 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
"""

import time
from machine import Pin, PWM

# GPIO numbers. ESP32: 25 and 26. ESP32-S3: 4 and 5. Pico: 14 and 15.
WARM_PIN = 4
COOL_PIN = 5

# Two thirds of full: the cap for VCC on 5V.
MAX_DUTY = 43690

# LOW first: nothing on the board holds the transistors off.
Pin(WARM_PIN, Pin.OUT, value=0)
Pin(COOL_PIN, Pin.OUT, value=0)
warm = PWM(Pin(WARM_PIN))
cool = PWM(Pin(COOL_PIN))
for led in (warm, cool):
    led.freq(1000)
    led.duty_u16(0)


def mix_kelvin(percent):
    # Averaged in mireds (a million over the kelvin): an approximation.
    s = percent / 100
    mired = (1 - s) * (1e6 / 3000) + s * (1e6 / 6500)
    return round(1e6 / mired / 100) * 100


while True:
    # The two duties always add up to MAX_DUTY: the brightness stays,
    # the share moves from warm to cool.
    for p in range(0, 101, 10):
        c = MAX_DUTY * p // 100
        w = MAX_DUTY - c
        warm.duty_u16(w)
        cool.duty_u16(c)
        print("cool", p, "%: about", mix_kelvin(p), "K")
        time.sleep(1.5)
