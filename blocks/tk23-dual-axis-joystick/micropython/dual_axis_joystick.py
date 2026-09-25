import sys
import time
from machine import ADC, Pin

# ESP32: 34 35 25. ESP32-S3: 4 5 7. Pico: 26 27 15.
X_PIN, Y_PIN, SW_PIN = 4, 5, 7
ESP = sys.platform == "esp32"

def analog(pin):
    adc = ADC(Pin(pin))
    if ESP:
        adc.atten(ADC.ATTN_11DB)      # the full range, to about 3.1 V
    return adc

x_adc, y_adc = analog(X_PIN), analog(Y_PIN)
sw = Pin(SW_PIN, Pin.IN)              # R10 on the board pulls it down
time.sleep_ms(200)                    # hands off the stick
cx, cy = x_adc.read_u16(), y_adc.read_u16()

while True:
    print("X", x_adc.read_u16() - cx, "Y", y_adc.read_u16() - cy,
          "SW", sw.value())
    time.sleep_ms(200)
