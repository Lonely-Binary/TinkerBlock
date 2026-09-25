from machine import Pin, I2C
import time

ADDRESS = 0x20                   # fixed on the TK85; the compact adapter ships here
KEYMAP = "DCBA#9630852*741"      # soft keypad
# KEYMAP = "D#0*C987B654A321"    # heavy-duty keypad

# Raspberry Pi Pico: I2C0 on GP0 (SDA) and GP1 (SCL). Change the pins for your board.
i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=100_000)

ONE = {0xE: 0, 0xD: 1, 0xB: 2, 0x7: 3}   # exactly one line low

def read(mask):
    i2c.writeto(ADDRESS, bytes([mask]))
    return i2c.readfrom(ADDRESS, 1)[0]

def get_key():
    upper = (read(0xF0) >> 4) & 0x0F   # which of P4-P7 the key pulled low
    lower = read(0x0F) & 0x0F          # which of P0-P3 the key pulled low
    if upper == 0x0F or lower == 0x0F:
        return None                    # no key
    if upper not in ONE or lower not in ONE:
        return "F"                     # two keys at once
    return KEYMAP[ONE[upper] + 4 * ONE[lower]]

last = None
while True:
    key = get_key()
    if key != last and key not in (None, "F"):
        print(key)
    last = key
    time.sleep_ms(20)
