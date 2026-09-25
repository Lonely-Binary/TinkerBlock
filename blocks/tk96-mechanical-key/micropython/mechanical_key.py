# Import required modules
from machine import Pin  # GPIO control
import time              # For delay (time.sleep)

# Pin number: change this to match your wiring
BUTTON_PIN = 0   # GPIO connected to BUTTON (e.g. GPIO 0)
LED_PIN = 1      # LED pin (GPIO 1, or external LED, Pico built-in LED is GPIO 25)

# Create pin objects
button = Pin(BUTTON_PIN, Pin.IN, Pin.PULL_UP)   # Set button pin as input mode, enable pull-up resistor
led = Pin(LED_PIN, Pin.OUT)                     # Set LED pin as output mode (to control LED on/off)

print("Mechanical key program started")
print("LED on when key pressed, LED off when key released")

# Main loop: runs forever
while True:
    # Read button state
    buttonState = button.value()  # Read button pin level: 0=pressed (LOW), 1=released (HIGH)
    
    # Control LED based on button state
    if buttonState == 0:
        # Key pressed: turn LED on and print message
        led.on()                 # Output HIGH, LED on
        print("Key pressed - LED on")
    else:
        # Key released: turn LED off
        led.off()                # Output LOW, LED off
        print("Key released - LED off")
    
    # Delay 100 milliseconds to avoid reading too fast
    time.sleep_ms(100)
