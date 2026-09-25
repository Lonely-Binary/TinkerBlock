"""
  TM1637 4-Digit Display - a clock that sets itself  TK51 / /p/tk51

  An ESP32 and a clock panel. The time comes over Wi-Fi from NTP.

  Wiring. No pad on this board is square, so go by the names
  printed on the back: GND, VCC, clock, data. Seen from the
  display side with the header on the left, GND is the top pin.

    GND   -> GND
    VCC   -> 3V3. Never 5V: the pull-ups would put 5 V on your
             pins.
    CLOCK -> GPIO 18
    DATA  -> GPIO 19

  Thonny
    Run > Configure interpreter   MicroPython (ESP32)
    Save tm1637.py from this folder to the board first.

  MicroPython keeps UTC and knows no timezones, so OFFSET_H is a
  fixed number of hours: change it by hand when daylight saving
  starts and ends. The Arduino clock.ino makes that switch itself.
"""

import time
import network
import ntptime
from tm1637 import TM1637, COLON, MINUS

SSID = "your-ssid"
PASSWORD = "your-password"
OFFSET_H = 10               # Melbourne in winter; 11 in summer

display = TM1637(18, 19)
display.brightness(4)
display.segments([MINUS] * 4)   # dashes until the time is real

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(SSID, PASSWORD)
while not wlan.isconnected():
    time.sleep_ms(250)
print(wlan.ifconfig()[0])

while True:
    try:
        ntptime.settime()   # sets the board's clock to UTC
        break
    except OSError:
        time.sleep(5)       # no answer yet: keep the dashes

while True:
    t = time.localtime(time.time() + OFFSET_H * 3600)
    colon = COLON if t[5] % 2 else 0
    # Leading zeros on, so midnight shows 00:00.
    display.number(t[3] * 100 + t[4], colon, True)
    time.sleep_ms(200)
