from machine import Pin, SPI
import time

# Pin number: change these to match your wiring
CS_PIN = 5     # GPIO connected to CS (e.g. GPIO 5)
DIN_PIN = 19   # GPIO connected to DIN (e.g. GPIO 19, SPI data)
CLK_PIN = 18   # GPIO connected to CLK (e.g. GPIO 18, SPI clock)

# MAX7219 register addresses
REG_NOOP = 0x00
REG_DECODE = 0x09
REG_INTENSITY = 0x0A
REG_SCAN_LIMIT = 0x0B
REG_SHUTDOWN = 0x0C
REG_DISPLAY_TEST = 0x0F

# Initialize SPI and CS pin
# Pico SPI pin mapping:
# SPI0: SCK=GPIO18, MOSI=GPIO19, MISO=GPIO16
spi = None
if CLK_PIN == 18 and DIN_PIN == 19:
    spi = SPI(0, baudrate=10000000, polarity=0, phase=0, sck=Pin(CLK_PIN), mosi=Pin(DIN_PIN))
else:
    try:
        spi = SPI(0, baudrate=10000000, polarity=0, phase=0, sck=Pin(CLK_PIN), mosi=Pin(DIN_PIN))
    except:
        spi = SPI(1, baudrate=10000000, polarity=0, phase=0, sck=Pin(CLK_PIN), mosi=Pin(DIN_PIN))

cs = Pin(CS_PIN, Pin.OUT)
cs.value(1)  # CS HIGH

# Pattern data (8×8 dot matrix, each pattern 8 bytes)
# Heart pattern
heart_pattern = [
    0b00000000,
    0b01100110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000
]

# Triangle pattern
triangle_pattern = [
    0b00000000,
    0b00010000,
    0b00111000,
    0b01111100,
    0b11111110,
    0b01111100,
    0b00111000,
    0b00010000
]

# Square pattern
square_pattern = [
    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11111111
]

# Circle pattern
circle_pattern = [
    0b00111100,
    0b01111110,
    0b11000011,
    0b10000001,
    0b10000001,
    0b11000011,
    0b01111110,
    0b00111100
]

# Star pattern
star_pattern = [
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00000000
]

# Arrow pattern
arrow_pattern = [
    0b00001000,
    0b00011100,
    0b00111110,
    0b01111111,
    0b00011100,
    0b00011100,
    0b00011100,
    0b00000000
]

# Smile pattern
smile_pattern = [
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100
]

# Pattern array for easy looping
patterns = [
    heart_pattern,      # 0: Heart
    triangle_pattern,   # 1: Triangle
    square_pattern,     # 2: Square
    circle_pattern,     # 3: Circle
    star_pattern,       # 4: Star
    arrow_pattern,      # 5: Arrow
    smile_pattern       # 6: Smile
]

pattern_names = [
    "Heart",
    "Triangle",
    "Square",
    "Circle",
    "Star",
    "Arrow",
    "Smile"
]

pattern_count = 7  # Number of patterns

# Reverse byte bit order (fix mirror display issue)
def reverse_byte(b):
    """Reverse byte bit order"""
    result = 0
    for i in range(8):
        result <<= 1
        result |= (b & 1)
        b >>= 1
    return result

# Send command to MAX7219
def max7219_write(register, data):
    """Write data to MAX7219"""
    cs.value(0)  # CS LOW
    # MAX7219 requires 16-bit data: high 8 bits are register address, low 8 bits are data
    spi.write(bytes([register, data]))
    cs.value(1)  # CS HIGH

# Initialize MAX7219
def max7219_init():
    """Initialize MAX7219 chip"""
    max7219_write(REG_DISPLAY_TEST, 0x00)  # Disable display test
    max7219_write(REG_SCAN_LIMIT, 0x07)    # Scan all 8 rows
    max7219_write(REG_DECODE, 0x00)        # No BCD decode
    max7219_write(REG_SHUTDOWN, 0x01)      # Normal mode (0x00=shutdown, 0x01=on)
    max7219_write(REG_INTENSITY, 0x08)     # Set brightness (0x00-0x0F)
    max7219_clear()                        # Clear display

# Clear display
def max7219_clear():
    """Clear all rows"""
    for i in range(1, 9):
        max7219_write(i, 0x00)

# Display pattern
def display_pattern(pattern):
    """Display pattern"""
    max7219_clear()  # Clear display
    for i in range(8):
        reversed_byte = reverse_byte(pattern[i])
        max7219_write(i + 1, reversed_byte)  # Row registers start from 1

print("8×8 matrix LED program started")
print("Cycling through various patterns: Heart, Triangle, Square, Circle, Star, Arrow, Smile")

# Initialize MAX7219
max7219_init()
print("MAX7219 initialization complete")

# Main loop: runs forever
while True:
    # Cycle through all patterns
    for i in range(pattern_count):
        display_pattern(patterns[i])
        print(f"Display pattern: {pattern_names[i]}")
        time.sleep(1)  # Switch pattern every second
