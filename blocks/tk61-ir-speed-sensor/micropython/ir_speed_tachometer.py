"""
  Infrared Speed Sensor - tachometer, MicroPython     TK61 / /p/tk61

  Wiring, the same as the first read:

    GND    -> GND
    VCC    -> 3V3   (the block pulls SIGNAL up to VCC with 10k)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Mounting: a slotted disc on the shaft, its rim through the slot
  so each bar breaks the beam once as it passes.

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Stop it with Ctrl-C.
"""

from machine import Pin, disable_irq, enable_irq
import time

# The GPIO number SIGNAL is wired to. ESP32: 25. ESP32-S3: 4. Pico: 15.
SENSOR_PIN = 4

SLOTS = 20          # openings in your disc: one pulse each, every turn
WINDOW_MS = 1000    # count for this long, then print
LOCKOUT_US = 500    # ignore a second edge this soon after the last

pulses = 0
last_edge = time.ticks_us()


def on_edge(pin):
    # Runs on every rising edge: a bar has just entered the beam.
    global pulses, last_edge
    at = time.ticks_us()
    if time.ticks_diff(at, last_edge) >= LOCKOUT_US:
        pulses += 1
        last_edge = at


sensor = Pin(SENSOR_PIN, Pin.IN)    # the board drives SIGNAL itself
sensor.irq(trigger=Pin.IRQ_RISING, handler=on_edge)

start = time.ticks_ms()
while True:
    time.sleep_ms(20)
    elapsed = time.ticks_diff(time.ticks_ms(), start)
    if elapsed < WINDOW_MS:
        continue

    state = disable_irq()           # take the count in one piece
    n = pulses
    pulses = 0
    enable_irq(state)
    start = time.ticks_add(start, elapsed)

    rpm = n * 1000 / elapsed * 60 / SLOTS
    print(n, "pulses  ", round(rpm), "rpm")
