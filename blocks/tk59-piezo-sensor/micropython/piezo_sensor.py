# Import required modules
from machine import Pin, ADC  # GPIO control and ADC
import time                   # For delay (time.sleep)

# Pin number: change this to match your wiring
PIEZO_PIN = 26  # GPIO connected to SIGNAL (e.g. GPIO 26, must be ADC pin)

# Create ADC object
piezo = ADC(Pin(PIEZO_PIN))  # Set piezoelectric sensor pin as ADC mode (to read analog signal)

print("Piezoelectric sensor program started")
print("Read pressure intensity value and output via serial")

# Main loop: runs forever
while True:
    # Read piezoelectric sensor analog value (0-65535)
    sensor_value = piezo.read_u16()  # Read sensor pin analog value: 0=no pressure, 65535=maximum pressure
    
    # Convert analog value to voltage value (0-3.3V)
    voltage = sensor_value * (3.3 / 65535.0)
    
    # Output intensity value
    print(f"Pressure intensity: {sensor_value} | Voltage: {voltage:.3f}V")
    
    # Delay 100 milliseconds to avoid reading too fast
    time.sleep_ms(100)
