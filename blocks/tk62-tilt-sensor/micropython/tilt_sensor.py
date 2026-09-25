# Import required modules
from machine import Pin  # GPIO control
import time              # For delay (time.sleep)

# Pin number: change these to match your wiring
TILT_PIN = 0    # GPIO connected to SIGNAL (e.g. GPIO 0)
LED_PIN = 1     # LED pin (GPIO 1, or external LED, Pico built-in LED is GPIO 25)

# Create pin objects
tilt = Pin(TILT_PIN, Pin.IN)   # Set tilt sensor pin as input (to read detection state)
led = Pin(LED_PIN, Pin.OUT)    # Set LED pin as output (to control LED on/off)

print("Tilt sensor program started")
print("LED on when tilt detected, LED off when level")

# Main loop: runs forever
while True:
    # Read tilt sensor state
    tiltState = tilt.value()   # Read sensor pin level: 1=tilt (HIGH), 0=level (LOW)
    
    # Control LED based on detection state
    if tiltState == 1:
        # Tilt detected: turn LED on and print message
        led.on()                 # Output HIGH, LED on
        print("Tilt detected - LED on")
    else:
        # Level state: turn LED off
        led.off()                # Output LOW, LED off
        print("Level state - LED off")
    
    # Delay 100 milliseconds to avoid reading too fast
    time.sleep_ms(100)
