# Import required modules
from machine import Pin, ADC  # GPIO control and ADC
import time                   # For delay (time.sleep)

# Pin number: change this to match your wiring
STEAM_PIN = 26   # GPIO connected to SIGNAL (e.g. GPIO 26, must be ADC pin)

# Create ADC object
steam = ADC(Pin(STEAM_PIN))  # Set steam sensor pin as ADC mode (to read analog signal)

# Filtering and change detection variables
lastValue = -1  # Previous value
CHANGE_THRESHOLD = 100  # Change threshold, only output if change exceeds this value

print("Steam sensor program started")
print("Reading analog value, higher value indicates higher steam concentration")
print("Only output when value changes significantly (reduce noise interference)")

# Main loop: runs forever
while True:
    # Multiple samples and average to reduce noise
    sum_value = 0
    for i in range(5):
        sum_value += steam.read_u16()
        time.sleep_us(100)
    steamValue = sum_value // 5  # Calculate average
    
    # Only output when value changes significantly (reduce random output in stable state)
    if lastValue == -1:
        # First reading, output directly
        print(f"Steam sensor value: {steamValue}")
        lastValue = steamValue
    else:
        # Calculate change amount
        change = abs(steamValue - lastValue)
        if change >= CHANGE_THRESHOLD:
            print(f"Steam sensor value: {steamValue} (change: {change})")
            lastValue = steamValue
    
    # Delay 200 milliseconds to avoid output too fast
    time.sleep_ms(200)
