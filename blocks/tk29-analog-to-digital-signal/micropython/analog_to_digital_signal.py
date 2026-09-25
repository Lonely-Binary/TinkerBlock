import sys
import time
from machine import ADC, Pin

# DIG_PIN, then ANA_PIN.
# ESP32: 25 and 34. ESP32-S3: 7 and 4. Pico: 15 and 26.
dig = Pin(7, Pin.IN)              # no pull: the block has its own
adc = ADC(Pin(4))
if sys.platform == "esp32":
    adc.atten(ADC.ATTN_11DB)      # the full range, to about 3.1 V

while True:
    print("DIG", dig.value(), "  ANA", adc.read_u16())
    time.sleep_ms(250)
