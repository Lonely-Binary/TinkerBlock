"""
  Piezo-Ceramic Sensor - a tap switch, MicroPython     TK59 / /p/tk59

  Each tap on the disc turns the TK01 XL LED on or off, and prints
  how hard it was.

  Wiring, the TK59. Count from the square pad, parts up, header on
  the left. Two wires: the block needs no supply.

    GND    -> GND
    NC     -> nothing   (connected to nothing on the board)
    NC     -> nothing   (nor is this one: there is no VCC pin)
    SIGNAL -> GPIO 34 on an ESP32, GPIO 4 on an ESP32-S3,
              GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way from its square pad:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Nothing to install: machine, sys and time are built in.
"""

import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
PIEZO_PIN = 4
# The TK01's SIGNAL. ESP32: 4. ESP32-S3: 5. Pico: 15.
LED_PIN = 5

# A tap is a rise of this much above the lowest reading since the
# last one. Lower it for lighter taps, raise it if the LED switches
# on its own.
RISE_MV = 150
# Deaf for this long after each tap, so one tap's ringing cannot
# switch the light twice.
LOCKOUT_MS = 150

adc = ADC(Pin(PIEZO_PIN))
ESP32 = sys.platform == "esp32"       # ESP32 and ESP32-S3
if ESP32:
    adc.atten(ADC.ATTN_11DB)          # the full range, to about 3.1 V
led = Pin(LED_PIN, Pin.OUT, value=0)


def read_mv():
    if ESP32:
        return adc.read_uv() // 1000  # calibrated in the chip
    return adc.read_u16() * 3300 // 65535


floor_mv = 0          # lowest reading since the last tap
armed = True
tap_at = 0
print("Tap the disc.")

while True:
    mv = read_mv()

    # Follow the level down as the last hump drains away, so a second
    # tap on its tail is measured from where it had got to.
    if mv < floor_mv:
        floor_mv = mv

    if armed and mv - floor_mv >= RISE_MV:
        # Follow the rise to its top, a few milliseconds at most.
        peak = mv
        start = time.ticks_ms()
        while time.ticks_diff(time.ticks_ms(), start) < 5:
            peak = max(peak, read_mv())

        led.value(not led.value())
        print("tap, peak", peak, "mV")

        armed = False
        tap_at = time.ticks_ms()
        floor_mv = peak

    since = time.ticks_diff(time.ticks_ms(), tap_at)
    if not armed and since >= LOCKOUT_MS:
        armed = True
