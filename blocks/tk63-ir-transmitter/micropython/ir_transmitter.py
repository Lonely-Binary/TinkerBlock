# Import required modules
from machine import Pin, PWM  # GPIO control and PWM
import time                   # For delay (time.sleep)

# Pin number: change this to match your wiring
IR_TX_PIN = 0  # GPIO connected to SIGNAL (e.g. GPIO 0)

# Create PWM object (for generating 38kHz frequency)
ir_tx = PWM(Pin(IR_TX_PIN))

# Set PWM frequency to 38kHz (common frequency for infrared remote control)
ir_tx.freq(38000)

print("Infrared transmitter program started")
print("Transmitting infrared signal...")

# Main loop: runs forever
while True:
    # Transmit infrared signal (38kHz modulation)
    # Set duty cycle to 50% (half of 65535)
    ir_tx.duty_u16(32767)  # 50% duty cycle
    time.sleep(0.1)        # Transmit for 100 milliseconds
    
    # Stop transmission
    ir_tx.duty_u16(0)      # 0% duty cycle, stop transmission
    time.sleep(1)          # Wait 1 second before transmitting again
    
    print("Infrared signal transmitted")
