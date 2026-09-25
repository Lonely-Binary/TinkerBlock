from machine import ADC, Pin
import sys
import time

# The GPIO number SIGNAL is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
pot = ADC(Pin(4))
if sys.platform == "esp32":       # both ESP32s
    pot.atten(ADC.ATTN_11DB)      # measure up to about 3.1 V

while True:
    reading = pot.read_u16()      # 0 .. 65535 on every board
    print(reading, reading * 100 // 65535, "%")
    time.sleep_ms(200)
