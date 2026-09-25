# Import required modules
from machine import Pin  # GPIO control
import time              # For delay (time.sleep)

# Pin number: change this to match your wiring
PHOTO_SWITCH_PIN = 0  # GPIO connected to SIGNAL (e.g. GPIO 0)
LED_PIN = 1           # LED pin (GPIO 1, or external LED, Pico built-in LED is GPIO 25)

# Create pin objects
photo_switch = Pin(PHOTO_SWITCH_PIN, Pin.IN)  # Set photoelectric switch pin as input (to read detection state)
led = Pin(LED_PIN, Pin.OUT)                    # Set LED pin as output (to control LED on/off)

print("Photoelectric switch program started")
print("LED on when object blocks light, LED off when no object detected")

# Main loop: runs forever
while True:
    # Read photoelectric switch state
    switch_state = photo_switch.value()  # Read sensor pin level: 1=object detected (HIGH), 0=no object (LOW)
    
    # Control LED based on detection state
    if switch_state == 1:
        # Object blocks light: turn LED on and print message
        led.on()            # Output HIGH, LED on
        print("Object blocks light - LED on")
    else:
        # No object detected: turn LED off
        led.off()           # Output LOW, LED off
        print("No object detected - LED off")
    
    # Delay 100 milliseconds to avoid reading too fast
    time.sleep_ms(100)
