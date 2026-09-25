import math, sys, time
from machine import ADC, Pin

# ESP32: 34. ESP32-S3: 4. Pico: 26.
adc = ADC(Pin(4))
ESP = sys.platform == "esp32"
if ESP:
    adc.atten(ADC.ATTN_11DB)                   # to about 3.1 V

while True:
    if ESP:
        ratio = adc.read_uv() / 1000000 / 3.3  # VCC taken as 3.3 V
    else:
        ratio = adc.read_u16() / 65535         # a fraction of VCC
    r = 10000 * ratio / (1 - ratio)
    c = 1 / (1 / 298.15 + math.log(r / 10000) / 3950) - 273.15
    print("%.1f C" % c)
    time.sleep(1)
