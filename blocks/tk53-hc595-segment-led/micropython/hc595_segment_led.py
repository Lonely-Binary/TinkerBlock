from machine import Pin
import time

# Pin number: change these to match your wiring
LATCH_PIN = 0   # GPIO connected to LATCH (e.g. GPIO 0)
CLOCK_PIN = 1   # GPIO connected to CLOCK (e.g. GPIO 1)
DATA_PIN = 2    # GPIO connected to DATA (e.g. GPIO 2)

# 7-segment display segment code table (common anode, numbers 0-9)
digit_patterns = [
    0b11111100,  # 0
    0b01100000,  # 1
    0b11011010,  # 2
    0b11110010,  # 3
    0b01100110,  # 4
    0b10110110,  # 5
    0b10111110,  # 6
    0b11100000,  # 7
    0b11111110,  # 8
    0b11110110   # 9
]

# Initialize pins
latch = Pin(LATCH_PIN, Pin.OUT)
clock = Pin(CLOCK_PIN, Pin.OUT)
data = Pin(DATA_PIN, Pin.OUT)

def display_digit(digit):
    """Display digit function"""
    if digit < 0 or digit > 9:
        return  # Check range
    
    # Latch pin LOW, ready to receive data
    latch.value(0)
    
    # Send data through shift register
    shift_out(data, clock, digit_patterns[digit])
    
    # Latch pin HIGH, output data to display
    latch.value(1)

def shift_out(data_pin, clock_pin, value):
    """Shift output function (LSBFIRST, least significant bit first)"""
    for i in range(8):
        # Send least significant bit
        data_pin.value(value & 0x01)
        # Clock rising edge
        clock_pin.value(1)
        time.sleep_us(1)
        clock_pin.value(0)
        time.sleep_us(1)
        # Shift right by one bit
        value >>= 1

print("74HC595 7-segment display program started")
print("Cycling through 0-9")

# Main loop: runs forever
while True:
    # Cycle through 0-9
    for i in range(10):
        display_digit(i)
        print(f"Display number: {i}")
        time.sleep(1)  # Switch number every second
