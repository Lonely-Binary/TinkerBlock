"""
  Dual Bright LEDs - a fade that looks even, MicroPython   TK95

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
STEPS = 100

# LOW first: nothing on the board holds the transistors off.
Pin(WARM_PIN, Pin.OUT, value=0)
Pin(COOL_PIN, Pin.OUT, value=0)
warm = PWM(Pin(WARM_PIN))
cool = PWM(Pin(COOL_PIN))
for led in (warm, cool):
    led.freq(1000)
    led.duty_u16(0)


def duty_for(step):
    # Through a 2.2 power, so equal steps look equal.
    return round((step / STEPS) ** 2.2 * MAX_DUTY)


def fade(led, name):
    print(name + ": up")
    for s in range(STEPS + 1):
        led.duty_u16(duty_for(s))
        time.sleep_ms(20)
    print(name + ": down")
    for s in range(STEPS, -1, -1):
        led.duty_u16(duty_for(s))
        time.sleep_ms(20)


while True:
    fade(warm, "3000K")
    fade(cool, "6500K")
