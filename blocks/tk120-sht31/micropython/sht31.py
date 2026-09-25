from machine import I2C, Pin
import time

# 0x2400: measure once, high repeatability, no clock stretching.
ADDR = 0x44
i2c = I2C(0, sda=Pin(8), scl=Pin(9), freq=100_000)

def crc8(data):
    crc = 0xFF
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = ((crc << 1) ^ 0x31) & 0xFF if crc & 0x80 else (crc << 1) & 0xFF
    return crc

def read():
    i2c.writeto(ADDR, b"\x24\x00")
    time.sleep_ms(20)                 # 15 ms is the datasheet maximum
    d = i2c.readfrom(ADDR, 6)         # temperature first, then humidity
    if d[2] != crc8(d[0:2]) or d[5] != crc8(d[3:5]):
        return None                   # a byte did not survive the wires
    t = -45 + 175 * ((d[0] << 8) | d[1]) / 65535
    rh = 100 * ((d[3] << 8) | d[4]) / 65535
    return t, rh

while True:
    r = read()
    print("%.2f C  %.2f %%" % r if r else "read failed")
    time.sleep(2)
