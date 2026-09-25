# Import required modules
from machine import Pin, ADC  # GPIO control and ADC
import time                   # For delay (time.sleep)

# Pin number: change this to match your wiring
SIGNAL_PIN = 26   # GPIO connected to SIGNAL (e.g. GPIO 26, must be ADC pin)

# Create ADC object
signal = ADC(Pin(SIGNAL_PIN))  # Set SIGNAL pin as ADC mode (to read analog signal)

print("Five-direction joystick program started")
print("Reading SIGNAL pin analog value to determine direction")

# Main loop: runs forever
while True:
    # Read SIGNAL pin analog value (0-65535)
    # Note: Pico ADC value is 0-65535, Arduino is 0-1023
    # Conversion formula: Pico value = Arduino value * 65535 / 1024 ≈ Arduino value * 64
    signal_value = signal.read_u16()
    
    # Determine joystick direction (based on actual measured values, converted to Pico ADC range)
    # Arduino value → Pico value:
    # Push up: 100-130 → 6400-8320
    # Push left: 270-290 → 17280-18560
    # Push down: 450-470 → 28800-30080
    # Push right: 1000-1024 → 64000-65535
    # Press: 180-190 → 11520-12160
    
    if signal_value >= 6400 and signal_value <= 8320:
        print("Push up")
    elif signal_value >= 17280 and signal_value <= 18560:
        print("Push left")
    elif signal_value >= 28800 and signal_value <= 30080:
        print("Push down")
    elif signal_value >= 64000 and signal_value <= 65535:
        print("Push right")
    elif signal_value >= 11520 and signal_value <= 12160:
        print("Press")
    else:
        print("Center position")
    
    # Print joystick status
    print(f"SIGNAL value: {signal_value}")
    
    time.sleep(0.2)  # Wait 200 milliseconds before reading again
