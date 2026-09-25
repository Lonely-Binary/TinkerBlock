import time
from machine import I2C, Pin

# SDA, SCL. ESP32: 21, 22. ESP32-S3: 8, 9. Pico: 4, 5.
i2c = I2C(0, sda=Pin(21), scl=Pin(22), freq=100_000)
EEPROM = 0x50                        # fixed on the TK31

i2c.writeto_mem(EEPROM, 0, bytes([123]), addrsize=16)
time.sleep_ms(5)                     # the chip is writing
print(i2c.readfrom_mem(EEPROM, 0, 1, addrsize=16)[0])
