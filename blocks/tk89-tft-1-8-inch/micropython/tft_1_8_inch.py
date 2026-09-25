from machine import Pin, SPI
import time

# Import ST7735 library
try:
    from st7735 import TFT, TFTColor
except ImportError:
    print("=" * 60)
    print("Error: st7735.py library file not found")
    print("=" * 60)
    print("Installation method:")
    print("1. Open st7735.py file in this folder in Thonny")
    print("2. File → Save As → Select 'MicroPython Device' → Save to Pico")
    print("3. Ensure st7735.py file is saved in Pico root directory or same directory as main program")
    print("=" * 60)
    raise ImportError("Please save st7735.py file to Pico first")

# ==================== Pin Definition ====================
TFT_CS = 5    # CS pin (chip select)
TFT_DC = 4    # DC pin (data/command selection)
TFT_RST = 3   # RST pin (reset)
TFT_BL = 2    # BL pin (backlight control)

SPI_MOSI = 19  # MOSI pin (SPI data)
SPI_SCK = 18   # SCK pin (SPI clock)

# ==================== Initialization ====================

# Initialize backlight
bl = Pin(TFT_BL, Pin.OUT)
bl.value(1)  # Turn on backlight

# Initialize SPI
# Pico SPI pin mapping:
# SPI0: SCK=GPIO18, MOSI=GPIO19, MISO=GPIO16
# SPI1: SCK=GPIO10, MOSI=GPIO11, MISO=GPIO12
spi = None
if SPI_SCK == 18 and SPI_MOSI == 19:
    # GPIO18 and GPIO19 correspond to SPI0
    spi = SPI(0, baudrate=10000000, polarity=0, phase=0, sck=Pin(SPI_SCK), mosi=Pin(SPI_MOSI))
elif SPI_SCK == 10 and SPI_MOSI == 11:
    # GPIO10 and GPIO11 correspond to SPI1
    spi = SPI(1, baudrate=10000000, polarity=0, phase=0, sck=Pin(SPI_SCK), mosi=Pin(SPI_MOSI))
else:
    # Other pin combinations, try SPI0 first, then SPI1 if fails
    try:
        spi = SPI(0, baudrate=10000000, polarity=0, phase=0, sck=Pin(SPI_SCK), mosi=Pin(SPI_MOSI))
    except:
        spi = SPI(1, baudrate=10000000, polarity=0, phase=0, sck=Pin(SPI_SCK), mosi=Pin(SPI_MOSI))

if spi is None:
    raise RuntimeError("SPI initialization failed")

# Create TFT display object
# TFT(spi, DC pin, RST pin, CS pin)
tft = TFT(spi, TFT_DC, TFT_RST, TFT_CS)

# ==================== Test Functions ====================

def test_fill_screen():
    """Test: Fill screen"""
    tft.fill(TFT.BLACK)
    time.sleep_ms(500)
    tft.fill(TFT.RED)
    time.sleep_ms(500)
    tft.fill(TFT.GREEN)
    time.sleep_ms(500)
    tft.fill(TFT.BLUE)
    time.sleep_ms(500)
    tft.fill(TFT.WHITE)
    time.sleep_ms(500)

def test_text():
    """Test: Text display (using rectangles instead, as text requires fonts)"""
    tft.fill(TFT.BLACK)
    tft.fillrect((0, 0), (128, 20), TFT.WHITE)
    tft.fillrect((0, 25), (128, 20), TFT.RED)
    tft.fillrect((0, 50), (128, 20), TFT.GREEN)
    tft.fillrect((0, 75), (128, 20), TFT.BLUE)
    time.sleep(2)

def test_colors():
    """Test: Color display"""
    tft.fill(TFT.BLACK)
    
    colors = [TFT.RED, TFT.GREEN, TFT.BLUE, TFT.YELLOW, TFT.PURPLE, TFT.CYAN, TFT.WHITE]
    w = 128 // 4
    h = 160 // 2
    
    for i in range(4):
        for j in range(2):
            idx = i + j * 4
            if idx < 7:
                tft.fillrect((i * w, j * h), (w, h), colors[idx])
    
    time.sleep(2)

def test_lines():
    """Test: Line drawing"""
    tft.fill(TFT.BLACK)
    for i in range(0, 128, 10):
        tft.line((0, 0), (i, 159), TFT.RED)
        time.sleep_ms(10)
    for i in range(0, 160, 10):
        tft.line((0, 0), (127, i), TFT.GREEN)
        time.sleep_ms(10)
    time.sleep(1)

def test_circles():
    """Test: Circle drawing"""
    tft.fill(TFT.BLACK)
    center_x, center_y = 64, 80
    max_r = min(center_x, center_y)
    for r in range(5, max_r, 5):
        tft.circle((center_x, center_y), r, TFT.BLUE)
        time.sleep_ms(50)
    time.sleep(1)

def test_rectangles():
    """Test: Rectangle drawing"""
    tft.fill(TFT.BLACK)
    max_size = min(128, 160)
    for i in range(0, max_size // 2, 5):
        tft.rect((i, i), (128 - 2 * i, 160 - 2 * i), TFT.YELLOW)
        time.sleep_ms(50)
    time.sleep(1)

def test_rotation():
    """Test: Screen rotation"""
    for rotation in range(4):
        tft.rotation(rotation)
        tft.fill(TFT.BLACK)
        tft.fillrect((10, 10), (50, 30), TFT.WHITE)
        time.sleep(2)
    tft.rotation(0)  # Restore default direction

def display_info():
    """Display information page"""
    tft.fill(TFT.BLACK)
    tft.fillrect((0, 0), (128, 15), TFT.WHITE)
    tft.fillrect((0, 20), (64, 10), TFT.RED)
    tft.fillrect((64, 20), (64, 10), TFT.GREEN)
    tft.fillrect((0, 35), (64, 10), TFT.BLUE)
    tft.fillrect((64, 35), (64, 10), TFT.YELLOW)
    tft.fillrect((0, 50), (64, 10), TFT.PURPLE)
    tft.fillrect((64, 50), (64, 10), TFT.CYAN)
    time.sleep(3)

# ==================== Main Program ====================
print("=" * 50)
print("TK89-1.8INCH TFT ST7735 Test Program")
print("Resolution: 128x160")
print("=" * 50)

# Initialize screen
# Try different initialization methods to find the version suitable for your screen
init_methods = [
    ("initr()", "Red tab version", lambda: tft.initr()),
    ("initb()", "Blue tab version", lambda: tft.initb()),
    ("initb2()", "Another blue tab version", lambda: tft.initb2()),
    ("initg()", "Green tab version", lambda: tft.initg()),
]

init_success = False
for method_name, description, method_func in init_methods:
    try:
        method_func()
        tft.rotation(0)  # Set rotation direction: 0=normal, 1=90°, 2=180°, 3=270°
        
        # Quick test: Fill colors
        tft.fill(TFT.RED)
        time.sleep_ms(200)
        tft.fill(TFT.GREEN)
        time.sleep_ms(200)
        tft.fill(TFT.BLUE)
        time.sleep_ms(200)
        tft.fill(TFT.BLACK)
        
        print(f"✓ {method_name} ({description}) initialization successful")
        init_success = True
        break
    except Exception as e:
        print(f"✗ {method_name} failed: {e}")
        continue

if not init_success:
    print("=" * 50)
    print("Warning: All initialization methods failed!")
    print("Please check wiring, power supply, and SPI configuration")
    print("=" * 50)
    raise RuntimeError("Screen initialization failed")

print("Screen initialization complete, starting tests...")
print()

# Display information page
display_info()

# Main loop
try:
    while True:
        print("Test: Fill screen")
        test_fill_screen()
        
        print("Test: Text display")
        test_text()
        
        print("Test: Color display")
        test_colors()
        
        print("Test: Line drawing")
        test_lines()
        
        print("Test: Circle drawing")
        test_circles()
        
        print("Test: Rectangle drawing")
        test_rectangles()
        
        print("Test: Screen rotation")
        test_rotation()
        
        print("Display information page")
        display_info()
        
        print("Test cycle complete, restarting...")
        print()
        time.sleep(2)

except KeyboardInterrupt:
    print("\nProgram interrupted by user")
except Exception as e:
    print("=" * 50)
    print("Program error occurred!")
    print(f"Error: {e}")
    print("=" * 50)
    import sys
    sys.print_exception(e)
