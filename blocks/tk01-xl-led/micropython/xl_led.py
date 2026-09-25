from machine import Pin
import time

# The GPIO number SIGNAL is wired to. ESP32, ESP32-S3: 4. Pico: 15.
led = Pin(4, Pin.OUT)

while True:
    led.on()      # HIGH: the pin supplies the LED's current
    time.sleep(0.5)
    led.off()
    time.sleep(0.5)
