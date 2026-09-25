# Import required modules
from machine import Pin, PWM  # GPIO control and PWM
import time                   # For delay (time.sleep)

# Pin number: change these to match your wiring
WARM_PIN = 1   # GPIO connected to 3000K (e.g. GPIO 1, must be PWM-capable pin)
COLD_PIN = 2   # GPIO connected to 6500K (e.g. GPIO 2)

# Create PWM objects
warm = PWM(Pin(WARM_PIN))
cold = PWM(Pin(COLD_PIN))

# Set PWM frequency (1000 Hz)
warm.freq(1000)
cold.freq(1000)

# Main loop: runs forever
while True:
    # Warm white (3000K brightest, 6500K off)
    warm.duty_u16(65535)    # Warm white brightest (65535 is max value)
    cold.duty_u16(0)         # Cool white off
    print("Warm white")
    time.sleep(2)
    
    # Cool white (3000K off, 6500K brightest)
    warm.duty_u16(0)
    cold.duty_u16(65535)
    print("Cool white")
    time.sleep(2)
    
    # Mixed light (both LEDs on, color temperature in middle)
    warm.duty_u16(32767)    # Warm white medium brightness
    cold.duty_u16(32767)     # Cool white medium brightness
    print("Mixed light")
    time.sleep(2)
