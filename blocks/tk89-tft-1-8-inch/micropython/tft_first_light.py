"""
  1.8-inch TFT Display - first light, MicroPython       TK89 / /p/tk89

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
    The driver is one file: ST7735.py from boochow's
    MicroPython-ST7735 on GitHub. Open it in Thonny and save it
    to the board under that name, then run this.
"""

import os
import sys
import time
import framebuf
from machine import SPI, Pin
from ST7735 import TFT

# The pins, by board. SCL and MO go to the SPI unit's clock and
# data out; the other four are any output pins.
if sys.platform == "rp2":                   # Raspberry Pi Pico
    SCL, MOSI, CS, DC, RST, BL = 18, 19, 5, 4, 3, 2
    SPI_ID = 0
elif "ESP32S3" in os.uname().machine:       # ESP32-S3
    SCL, MOSI, CS, DC, RST, BL = 12, 11, 10, 2, 42, 41
    SPI_ID = 1
else:                                       # ESP32
    SCL, MOSI, CS, DC, RST, BL = 18, 23, 15, 2, 4, 32
    SPI_ID = 2

# The backlight first: Q2 on the board keeps it off until BL is
# HIGH, so a dark screen here is BL, not the picture.
backlight = Pin(BL, Pin.OUT, value=1)

# 10 MHz: under the controller's rated 15 MHz for writes.
spi = SPI(SPI_ID, baudrate=10_000_000, polarity=0, phase=0,
          sck=Pin(SCL), mosi=Pin(MOSI))
tft = TFT(spi, DC, RST, CS)

# The panel's type. If the colours come out wrong or there is a
# band of noise at one edge, try tft.initg() or tft.initb().
tft.initr()
tft.rgb(True)            # False if red and blue come out swapped
# tft.invertcolor(True)  # if BLACK shows as white
tft.rotation(1)          # 1 or 3: landscape. 0 or 2: portrait.
W, H = tft.size()


def swap(colour):
    # framebuf stores a pixel low byte first; the display wants
    # the high byte first.
    return ((colour & 0xFF) << 8) | (colour >> 8)


def label(x, y, text, ink, paper):
    # Draw text into a small buffer, then send just that
    # rectangle: MicroPython's framebuf has an 8 x 8 font.
    w, h = 8 * len(text), 8
    buf = bytearray(w * h * 2)
    fb = framebuf.FrameBuffer(buf, w, h, framebuf.RGB565)
    fb.fill(swap(paper))
    fb.text(text, 0, 0, swap(ink))
    tft.image(x, y, x + w - 1, y + h - 1, buf)


PANELS = [
    (TFT.RED, "RED", TFT.WHITE),
    (TFT.GREEN, "GREEN", TFT.BLACK),
    (TFT.BLUE, "BLUE", TFT.WHITE),
    (TFT.WHITE, "WHITE", TFT.BLACK),
    (TFT.BLACK, "BLACK", TFT.WHITE),
]

print("TK89 first light:", W, "x", H)
while True:
    for colour, name, ink in PANELS:
        tft.fill(colour)
        label(8, 8, name, ink, colour)
        print("now showing:", name)
        time.sleep(1.5)
    label(8, 24, "%d x %d pixels" % (W, H), TFT.WHITE, TFT.BLACK)
    tft.rect((0, 0), (W, H), TFT.YELLOW)
    time.sleep(3)
