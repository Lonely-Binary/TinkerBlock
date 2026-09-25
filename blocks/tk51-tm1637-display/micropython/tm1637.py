"""
  TM1637 driver for the TK51 4-Digit Display       TK51 / /p/tk51

  Save this file to the board as tm1637.py (Thonny: File > Save
  as > MicroPython device). The other scripts in this folder
  import it. It drives the two wires by hand, the way the Arduino
  library does: a pin is pulled to 0 V as an output, or let go as
  an input so the board's 10 kohm resistor pulls it up to VCC.

  Nothing to install: machine and time are built in.
"""

import time
from machine import Pin

# Bit 0 is segment A (top), bit 6 is G (middle), bit 7 the point,
# or on a clock panel the second digit's bit is the colon.
DIGITS = (0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
          0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71)
MINUS = 0x40
COLON = 0b01000000          # the dots mask for the second digit


class TM1637:
    def __init__(self, clock, data, delay_us=100):
        self.clk = Pin(clock, Pin.IN)
        self.dio = Pin(data, Pin.IN)
        self.us = delay_us
        self.level = 0x08 | 4       # display on, brightness 4

    def _low(self, p):
        p.init(Pin.OUT, value=0)    # pull the wire to 0 V

    def _let_go(self, p):
        p.init(Pin.IN)              # the pull-up takes it high

    def _wait(self):
        time.sleep_us(self.us)

    def _start(self):               # DATA falls while CLOCK is high
        self._low(self.dio)
        self._wait()

    def _stop(self):                # DATA rises while CLOCK is high
        self._low(self.dio)
        self._wait()
        self._let_go(self.clk)
        self._wait()
        self._let_go(self.dio)
        self._wait()

    def _byte(self, b):
        for _ in range(8):          # least significant bit first
            self._low(self.clk)
            self._wait()
            if b & 1:
                self._let_go(self.dio)
            else:
                self._low(self.dio)
            self._wait()
            self._let_go(self.clk)
            self._wait()
            b >>= 1
        self._low(self.clk)         # ninth clock: the chip answers
        self._let_go(self.dio)
        self._wait()
        self._let_go(self.clk)
        self._wait()
        ack = self.dio.value() == 0
        if ack:
            self._low(self.dio)
        self._wait()
        self._low(self.clk)
        self._wait()
        return ack

    def brightness(self, level, on=True):
        """0 to 7. Sent with the next write, not on its own."""
        self.level = (0x08 if on else 0) | (level & 7)

    def segments(self, segs, pos=0):
        """One byte per digit, starting at pos (0 is the left)."""
        self._start()
        self._byte(0x40)            # write, address steps itself
        self._stop()
        self._start()
        self._byte(0xC0 | (pos & 3))
        for s in segs:
            self._byte(s)
        self._stop()
        self._start()
        self._byte(0x80 | self.level)
        self._stop()

    def number(self, n, dots=0, leading=False, length=4, pos=0):
        """A whole number in `length` digits from `pos`. dots is
        counted from the left of this call: 0b10000000 is its
        first digit, 0b01000000 its second."""
        neg = n < 0
        n = abs(n)
        room = length - 1 if neg else length
        if n >= 10 ** room:
            raise ValueError("does not fit in %d digits" % length)
        segs = []
        for i in range(length):
            if n == 0 and i > 0 and not leading:
                segs.append(0)
            else:
                segs.append(DIGITS[n % 10])
            n //= 10
        segs.reverse()
        if neg:                     # the blank nearest the number
            i = 0
            while i + 1 < length and segs[i + 1] == 0:
                i += 1
            segs[i] = MINUS
        for i in range(length):
            if dots & (0x80 >> i):
                segs[i] |= 0x80
        self.segments(segs, pos)
