import sys
import time
from machine import ADC, Pin

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
adc = ADC(Pin(4))
if sys.platform == "esp32":
    adc.atten(ADC.ATTN_11DB)      # the full range, to about 3.1 V

while True:
    total = 0
    n = 0
    start = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), start) < 50:
        total += adc.read_u16()   # more light, higher
        n += 1
    print(total // n)
    time.sleep_ms(200)
