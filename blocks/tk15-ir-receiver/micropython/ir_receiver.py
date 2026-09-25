# mpremote mip install "github:peterhinch/micropython_ir/ir_rx"
import time
from machine import Pin
from ir_rx.nec import NEC_8

# ESP32: 23. ESP32-S3: 9. Pico: 16.
IR_RX_PIN = 9

def got(command, address, ctrl):
    if command < 0:                    # a repeat frame
        print("[repeat]")
    else:
        print("address=0x%02x  command=0x%02x" % (address, command))

ir = NEC_8(Pin(IR_RX_PIN, Pin.IN), got)
while True:
    time.sleep_ms(500)
