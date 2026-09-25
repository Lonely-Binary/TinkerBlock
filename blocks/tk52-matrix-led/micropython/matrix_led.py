# 8x8 LED Matrix - the first picture               TK52 / /p/tk52
#
# Wiring. Matrix up, header at the bottom, the pins read
# GND VCC CS DIN NC CLK from the left. There is no square pad
# on this board: count from GND, printed on both sides.
#
#   GND -> GND
#   VCC -> 5V (VBUS on a Pico). Never 3V3: the MAX7219 needs
#          4.0 to 5.5 V.
#   CS  -> GPIO 5 on an ESP32, GPIO 10 on an ESP32-S3,
#          GP17 on a Raspberry Pi Pico
#   DIN -> GPIO 23 on an ESP32, GPIO 11 on an ESP32-S3,
#          GP19 on a Raspberry Pi Pico
#   NC  -> nothing: no trace on the board reaches it
#   CLK -> GPIO 18 on an ESP32, GPIO 12 on an ESP32-S3,
#          GP18 on a Raspberry Pi Pico
#
# Save it to the board with Thonny and run it. No library needed.

from machine import Pin
import time

# ESP32: 5, 23, 18. ESP32-S3: 10, 11, 12. Pico: 17, 19, 18.
cs = Pin(5, Pin.OUT, value=1)
din = Pin(23, Pin.OUT, value=0)
clk = Pin(18, Pin.OUT, value=0)

# If the picture comes out on its side, try 1, 2 or 3 quarter
# turns. If it comes out mirrored, set FLIP to True.
TURN = 0
FLIP = False

# Pictures: the top row first, the leftmost pixel in the top bit.
HEART = (0b00000000, 0b01100110, 0b11111111, 0b11111111,
         0b11111111, 0b01111110, 0b00111100, 0b00011000)
ARROW = (0b00011000, 0b00111100, 0b01111110, 0b11011011,
         0b00011000, 0b00011000, 0b00011000, 0b00011000)


def send(reg, data):
    # One 16-bit message: address, then data, top bit first.
    # CS going back high is what makes it take effect.
    word = (reg << 8) | data
    cs.value(0)
    for i in range(15, -1, -1):
        din.value((word >> i) & 1)
        clk.value(1)
        clk.value(0)
    cs.value(1)


def lit(pic, x, y):
    # Is the picture's pixel at (x, y) lit? x from the left, y down.
    if FLIP:
        x = 7 - x
    for _ in range(TURN):
        x, y = y, 7 - x
    return pic[y] & (0x80 >> x)


def show(pic):
    # On the TK52, register d + 1 lights column d from the left,
    # and bit b of it lights the pixel b rows down from the top.
    for d in range(8):
        column = 0
        for b in range(8):
            if lit(pic, d, b):
                column |= 1 << b
        send(1 + d, column)


send(0x0F, 0)  # display test off
send(0x09, 0)  # no decoding: every bit is one LED
send(0x0B, 7)  # scan all eight columns, never fewer
send(0x0A, 4)  # brightness 4 of 15
send(0x0C, 1)  # wake up: it powers up in shutdown

while True:
    show(HEART)
    print("heart")
    time.sleep(1)
    show(ARROW)
    print("arrow")
    time.sleep(1)
