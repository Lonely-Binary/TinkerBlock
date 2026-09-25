"""
  4-Direction Tilt Sensor - tip-over alarm, MicroPython  TK19 / /p/tk19

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND  -> GND
    VCC  -> 3V3   (never 5V beside a 3.3 V board)
    A    -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3, GP10 on a Pico
    B    -> GPIO 26, GPIO 5, GP11      (same order)
    C    -> GPIO 27, GPIO 6, GP12
    D    -> GPIO 32, GPIO 7, GP13

  The block stands on edge on the box's wall, header edge down.

  A TK01 XL LED on a second pin is the alarm (optional):
    GND    -> GND
    SIGNAL -> GPIO 4 on an ESP32, GPIO 15 on an ESP32-S3, GP15 on a Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO numbers A, B, C and D are wired to.
# ESP32: 25 26 27 32. ESP32-S3: 4 5 6 7. Pico: 10 11 12 13.
PIN_A, PIN_B, PIN_C, PIN_D = 4, 5, 6, 7
# The GPIO number the TK01's SIGNAL is wired to.
# ESP32: 4. ESP32-S3: 15. Pico: 15.
LED_PIN = 15

STABLE_READS = 5                # 5 scans, 10 ms apart: 50 ms
SCAN_MS = 10
TIP_MS = 1000
UPRIGHT = "header"              # standing on edge, header down

a = Pin(PIN_A, Pin.IN, Pin.PULL_UP)
b = Pin(PIN_B, Pin.IN, Pin.PULL_UP)
c = Pin(PIN_C, Pin.IN, Pin.PULL_UP)
d = Pin(PIN_D, Pin.IN, Pin.PULL_UP)
led = Pin(LED_PIN, Pin.OUT, value=0)


def release_all():
    for p in (a, b, c, d):
        p.init(Pin.IN, Pin.PULL_UP)


def hold_low(p):
    p.init(Pin.OUT, value=0)


def scan_tilt():
    release_all()
    hold_low(a)
    time.sleep_us(10)
    ab = b.value() == 0
    da = d.value() == 0

    release_all()
    hold_low(c)
    time.sleep_us(10)
    bc = b.value() == 0
    cd = d.value() == 0

    release_all()
    if ab:
        return "left"
    if da:
        return "header"
    if bc:
        return "top"
    if cd:
        return "right"
    return "none"


candidate = UPRIGHT
count = 0
believed = UPRIGHT
tipped_at = None                # None while upright
alarm_on = False

while True:
    side = scan_tilt()
    if side == candidate:
        count = min(count + 1, STABLE_READS)
    else:
        candidate = side
        count = 1
    if count >= STABLE_READS and candidate != believed:
        believed = candidate
        print(believed)

    now = time.ticks_ms()
    if believed == UPRIGHT:
        tipped_at = None        # upright again: stop the clock
    elif tipped_at is None:
        tipped_at = now         # just tipped: start the clock

    if (tipped_at is not None and not alarm_on
            and time.ticks_diff(now, tipped_at) >= TIP_MS):
        alarm_on = True         # latched: only a reset clears it
        led.value(1)
        print("tipped over: alarm")
    time.sleep_ms(SCAN_MS)
