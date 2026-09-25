import sys
import time
from machine import ADC, Pin

# ESP32: 34 35 32 25. ESP32-S3: 4 5 6 7. Pico: 26 27 28 15.
X_PIN, Y_PIN, BTNS_PIN, KEY_PIN = 4, 5, 6, 7
BUTTON_NAME = ("U11", "U10", "U5", "U7", "U8", "U6")
LIMIT_MV = (2475, 1375, 963, 743, 605, 275)
ESP = sys.platform == "esp32"

def analog(pin):
    adc = ADC(Pin(pin))
    if ESP:
        adc.atten(ADC.ATTN_11DB)      # the full range, to about 3.1 V
    return adc

x_adc, y_adc, btns = analog(X_PIN), analog(Y_PIN), analog(BTNS_PIN)
key = Pin(KEY_PIN, Pin.IN)            # R1 on the board pulls it down

while True:
    mv = btns.read_uv() / 1000 if ESP else btns.read_u16() * 3300 / 65535
    b = next((n for n, lim in zip(BUTTON_NAME, LIMIT_MV) if mv > lim), "none")
    print("X", x_adc.read_u16(), "Y", y_adc.read_u16(), "KEY", key.value(), b)
    time.sleep_ms(200)
