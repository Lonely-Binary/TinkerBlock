"""
  Disc Potentiometer - a percentage and a brightness  TK07 / /p/tk07

  Wiring. Count from the square pad on each TinkerBlock board, header
  at the bottom. TK07 Disc Potentiometer:

    GND    -> GND
    VCC    -> 3V3   (never 5V: at one stop, SIGNAL gives your pin VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  TK01 XL LED: GND -> GND, both NC -> nothing, and
    SIGNAL -> GPIO 25 on an ESP32, GPIO 5 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine, sys and time are built in.
"""

from machine import ADC, PWM, Pin
import sys
import time

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
POT_PIN = 4
# The GPIO number the TK01's SIGNAL is on. ESP32: 25. ESP32-S3: 5. Pico: 15.
LED_PIN = 5

pot = ADC(Pin(POT_PIN))
if sys.platform == "esp32":         # both ESP32s report "esp32"
    pot.atten(ADC.ATTN_11DB)        # measure up to about 3.1 V

led = PWM(Pin(LED_PIN))
led.freq(1000)

while True:
    reading = pot.read_u16()        # 0 .. 65535
    led.duty_u16(reading)           # the same range: no scaling needed
    print(reading * 100 // 65535, "%")
    time.sleep_ms(50)
