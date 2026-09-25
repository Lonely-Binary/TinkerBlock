import dht, machine, time

# ESP32: 18. ESP32-S3: 4. Pico: 2.
sensor = dht.DHT11(machine.Pin(4))
time.sleep(1)

while True:
    try:
        sensor.measure()
        print(sensor.temperature(), "C ", sensor.humidity(), "%")
    except Exception:   # no answer, or a bad checksum
        print("read failed")
    time.sleep(2)
