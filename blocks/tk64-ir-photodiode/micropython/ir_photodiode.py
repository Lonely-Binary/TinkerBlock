# Note: This program requires micropython-ir library
# Installation: Copy ir.py file to Pico

# Import required modules
from machine import Pin  # GPIO control
import time              # For delay (time.sleep)

# Pin number: change this to match your wiring
IR_RX_PIN = 0  # GPIO connected to SIGNAL (e.g. GPIO 0)

# Create pin object
ir_rx = Pin(IR_RX_PIN, Pin.IN)  # Set infrared receiver pin as input mode

print("Infrared receiver program started")
print("Waiting to receive infrared signal...")

# Variables
last_state = 1  # Previous pin state (default HIGH)

# Main loop: runs forever
while True:
    # Read pin state
    current_state = ir_rx.value()
    
    # Detect signal change (HIGH to LOW indicates signal received)
    if current_state == 0 and last_state == 1:
        print("Infrared signal detected")
        # Note: Complete decoding requires micropython-ir library
        # This is just simple signal change detection
    
    # Update previous state
    last_state = current_state
    
    time.sleep(0.01)  # Brief delay
