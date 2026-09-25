from machine import ADC, Pin
import sys
import time

# The GPIO number SIG is wired to. ESP32: 34. ESP32-S3: 4. Pico: 26.
adc = ADC(Pin(34))

if sys.platform == "rp2":
    def sig_volts():
        return adc.read_u16() * 3.3 / 65535
else:
    adc.atten(ADC.ATTN_11DB)          # full range; the default is ~1 V
    def sig_volts():
        return adc.read_uv() / 1_000_000

while True:
    print("{:.2f} V".format(sig_volts() * 5.0))   # the terminal is 5 x SIG
    time.sleep(0.5)
