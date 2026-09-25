import time
from machine import Pin, PWM

# A PWM pin. ESP32: 4. ESP32-S3: 4. Pico: 15.
motor = PWM(Pin(4), freq=1000)
motor.duty_u16(0)
KICK_MS = 50              # full power first, so low levels start

def buzz(level, ms):
    motor.duty_u16(65535)             # kick: sure to start
    time.sleep_ms(KICK_MS)
    motor.duty_u16(level * 257)       # then the strength you want
    time.sleep_ms(ms)
    motor.duty_u16(0)

while True:
    buzz(255, 300)        # strong
    time.sleep_ms(700)
    buzz(150, 300)        # gentler
    time.sleep_ms(700)
