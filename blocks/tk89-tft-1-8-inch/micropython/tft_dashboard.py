"""
  1.8-inch TFT Display - redraw only what changed, MicroPython
                                                        TK89 / /p/tk89

  Wiring. The eight pins top to bottom, screen facing you, header on
  the left (the front prints BL RST DC SCL MO CS 3V3 GND):

    BL   -> GPIO 41 on an ESP32-S3, GPIO 32 on an ESP32, GP2 on a
            Pico. The backlight is off until BL is HIGH.
    RST  -> GPIO 42 on an ESP32-S3, GPIO 4 on an ESP32, GP3 on a Pico
    DC   -> GPIO 2 on an ESP32-S3, GPIO 2 on an ESP32, GP4 on a Pico
    SCL  -> GPIO 12 on an ESP32-S3, GPIO 18 on an ESP32, GP18 on a
            Pico (the SPI clock)
    MO   -> GPIO 11 on an ESP32-S3, GPIO 23 on an ESP32, GP19 on a
            Pico (MOSI, SPI data out)
    CS   -> GPIO 10 on an ESP32-S3, GPIO 15 on an ESP32, GP5 on a Pico
    3V3  -> 3V3   (never 5V: the display is a 3.3 V part)
    GND  -> GND

  Thonny
    Run > Configure interpreter   MicroPython (ESP32) or
                                  MicroPython (Raspberry Pi Pico)
    Needs ST7735.py (boochow's MicroPython-ST7735) saved on the
    board, as in first light. Stop it with Ctrl-C.
"""

import os
import sys
import time
import framebuf
from machine import SPI, Pin
from ST7735 import TFT

if sys.platform == "rp2":                   # Raspberry Pi Pico
    SCL, MOSI, CS, DC, RST, BL = 18, 19, 5, 4, 3, 2
    SPI_ID = 0
elif "ESP32S3" in os.uname().machine:       # ESP32-S3
    SCL, MOSI, CS, DC, RST, BL = 12, 11, 10, 2, 42, 41
    SPI_ID = 1
else:                                       # ESP32
    SCL, MOSI, CS, DC, RST, BL = 18, 23, 15, 2, 4, 32
    SPI_ID = 2

# True: clear the whole screen and draw everything, every frame.
# Try it once, watch the flicker, and read the time it prints.
CLEAR_EVERY_FRAME = False

BG, INK, BAR = TFT.BLACK, TFT.WHITE, TFT.GREEN
BAR_X, BAR_Y, BAR_W, BAR_H = 10, 90, 140, 14

backlight = Pin(BL, Pin.OUT, value=1)   # off until BL is HIGH
spi = SPI(SPI_ID, baudrate=10_000_000, polarity=0, phase=0,
          sck=Pin(SCL), mosi=Pin(MOSI))
tft = TFT(spi, DC, RST, CS)
tft.initr()          # see first light if the colours are off
tft.rotation(1)      # landscape, 160 x 128
# tft.invertcolor(True)  # if first light needed it


def swap(colour):
    return ((colour & 0xFF) << 8) | (colour >> 8)


def label(x, y, text, ink, paper):
    # Text into a small buffer with its own background, then send
    # only that rectangle. The old text is painted over, not
    # cleared first, so nothing is blank for a moment.
    w, h = 8 * len(text), 8
    buf = bytearray(w * h * 2)
    fb = framebuf.FrameBuffer(buf, w, h, framebuf.RGB565)
    fb.fill(swap(paper))
    fb.text(text, 0, 0, swap(ink))
    tft.image(x, y, x + w - 1, y + h - 1, buf)


def draw_frame():
    # The parts that never change: drawn once.
    tft.fill(BG)
    label(10, 10, "TK89  uptime", INK, BG)
    tft.rect((BAR_X - 1, BAR_Y - 1), (BAR_W + 2, BAR_H + 2), INK)


bar_was = 0


def draw_value(tenths):
    global bar_was
    label(10, 44, "%6d.%d s" % (tenths // 10, tenths % 10), INK, BG)
    # The bar: paint only the strip between the old and new ends.
    bar = (tenths % 100) * BAR_W // 100
    if bar > bar_was:
        tft.fillrect((BAR_X + bar_was, BAR_Y), (bar - bar_was, BAR_H),
                     BAR)
    elif bar < bar_was:
        tft.fillrect((BAR_X + bar, BAR_Y), (bar_was - bar, BAR_H), BG)
    bar_was = bar


draw_frame()
start = time.ticks_ms()
last_print = start
while True:
    tenths = time.ticks_diff(time.ticks_ms(), start) // 100
    t0 = time.ticks_us()
    if CLEAR_EVERY_FRAME:
        draw_frame()
        bar_was = 0
    draw_value(tenths)
    took = time.ticks_diff(time.ticks_us(), t0)
    if time.ticks_diff(time.ticks_ms(), last_print) >= 1000:
        last_print = time.ticks_ms()
        print("everything:" if CLEAR_EVERY_FRAME else "changes:",
              took, "us")
    time.sleep_ms(100)
