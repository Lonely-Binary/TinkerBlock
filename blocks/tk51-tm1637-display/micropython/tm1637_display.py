from machine import Pin
import time

# Raspberry Pi Pico: CLOCK on GP0, DATA on GP1. Change for your board.
clk = Pin(0, Pin.IN)     # released — the board's 10 kohm resistor holds it high
dio = Pin(1, Pin.IN)

DIGITS = (0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F)

def low(p):
    p.init(Pin.OUT, value=0)         # pull the wire down to 0 V
def high(p):
    p.init(Pin.IN)                   # let go, and the resistor pulls it up
def pause():
    time.sleep_us(100)

def write_byte(b):
    for _ in range(8):               # least significant bit first, unlike I2C
        low(clk); pause()
        if b & 1:
            high(dio)
        else:
            low(dio)
        pause()
        high(clk); pause()
        b >>= 1
    low(clk); high(dio); pause()     # ninth clock: the chip answers
    high(clk); pause()
    ack = dio.value() == 0
    if ack:
        low(dio)
    pause()
    low(clk); pause()
    return ack

def start():
    low(dio); pause()

def stop():
    low(dio); pause()
    high(clk); pause()
    high(dio); pause()

def show(segments, brightness=4):
    start(); write_byte(0x40); stop()                # write, address steps itself
    start(); write_byte(0xC0)                        # begin at the leftmost digit
    for s in segments:
        write_byte(s)
    stop()
    start(); write_byte(0x88 | brightness); stop()   # display on, brightness 0-7

def number(n, colon=False):
    segs = [DIGITS[int(c)] for c in "%04d" % (n % 10000)]
    if colon:
        segs[1] |= 0x80              # the colon is the second digit's point
    show(segs)

while True:
    number(1234, colon=True)
    time.sleep(1)
    number(1234)
    time.sleep(1)
