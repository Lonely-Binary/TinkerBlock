from machine import Pin, time_pulse_us
import time

trig = Pin(9, Pin.OUT)
echo = Pin(10, Pin.IN)

while True:
    trig.off(); time.sleep_us(2)
    trig.on();  time.sleep_us(500)
    trig.off()

    us = time_pulse_us(echo, 1, 25000)   # returns negative on timeout
    print("no echo" if us < 0 else "%.1f cm" % ((us / 2) * 0.0343))

    time.sleep_ms(100)
