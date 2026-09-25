"""
  Knock Sensor - a secret knock, MicroPython          TK28 / /p/tk28

  Wiring, the TK28. Count from the square pad, switch at the top,
  header at the bottom:

    GND    -> GND
    VCC    -> 3V3   (never 5V: during a knock, SIGNAL gives your pin VCC)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> GPIO 4 on an ESP32, GPIO 5 on an ESP32-S3,
              GP14 on a Raspberry Pi Pico

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Save it to the board as main.py to run it on every power-up.
    Nothing to install: machine and time are built in.
"""

from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
KNOCK_PIN = 4
# The TK01's SIGNAL. ESP32: 4. ESP32-S3: 5. Pico: 14.
LED_PIN = 5

HOLD_OFF_MS = 100                   # longer than the spring rings
END_MS = 1500                       # this much silence ends a try
OPEN_MS = 3000                      # how long the LED stays on
TOLERANCE = 0.1                     # of the longest gap
SECRET = [2, 1, 1, 2, 4, 2]         # shave and a haircut, two bits
MAX_KNOCKS = 10

knock = Pin(KNOCK_PIN, Pin.IN)      # no pull: the block has its own
led = Pin(LED_PIN, Pin.OUT, value=0)
times = []
last_knock = time.ticks_add(time.ticks_ms(), -HOLD_OFF_MS)

def on_knock(pin):                  # short, and no printing
    global last_knock
    now = time.ticks_ms()
    if time.ticks_diff(now, last_knock) < HOLD_OFF_MS:
        return                      # still ringing
    last_knock = now
    if len(times) < MAX_KNOCKS:
        times.append(now)

def matches(t):
    if len(t) != len(SECRET) + 1:
        return False                # wrong count
    gaps = [time.ticks_diff(t[i + 1], t[i]) for i in range(len(SECRET))]
    longest = max(gaps)
    for heard, wanted in zip(gaps, SECRET):
        if abs(heard / longest - wanted / max(SECRET)) > TOLERANCE:
            return False
    return True

knock.irq(trigger=Pin.IRQ_RISING, handler=on_knock)

while True:
    quiet = time.ticks_diff(time.ticks_ms(), last_knock)
    if times and quiet > END_MS:    # the knocking stopped
        heard = times
        times = []                  # ready for the next try
        ok = matches(heard)
        print(len(heard), "knocks:", "open" if ok else "wrong")
        if ok:
            led.value(1)
            time.sleep_ms(OPEN_MS)
            led.value(0)
    time.sleep_ms(10)
