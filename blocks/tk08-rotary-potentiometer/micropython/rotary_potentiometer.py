from machine import ADC, Pin
import sys
import time

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
pot = ADC(Pin(4))
if sys.platform == "esp32":
    pot.atten(ADC.ATTN_11DB)   # widest range, roughly 0 to 3.1 V

while True:
    print(pot.read_u16())      # 0 to 65535
    time.sleep_ms(200)
