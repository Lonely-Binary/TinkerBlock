# mpremote mip install "github:peterhinch/micropython_ir/ir_tx"
# Sending works on an ESP32 or a Pico; not on an ESP8266.
import time
from machine import Pin
from ir_tx.nec import NEC

# ESP32: 22. ESP32-S3: 6. Pico: 17.
IR_TX_PIN = 6

ADDRESS = 0x04                   # REPLACE: from your own remote
COMMAND = 0x16                   # REPLACE: from your own remote

nec = NEC(Pin(IR_TX_PIN, Pin.OUT, value=0))   # LOW: LED off
while True:
    nec.transmit(ADDRESS, COMMAND)
    print("sent")
    time.sleep(2)
