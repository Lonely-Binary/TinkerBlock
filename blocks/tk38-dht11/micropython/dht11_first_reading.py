import dht, machine, time

# The pin DATA is wired to. ESP32: 18. ESP32-S3: 4. Pico: 2.
sensor = dht.DHT11(machine.Pin(4))
time.sleep(1)                   # nothing for 1 s after power-up

while True:
    try:
        sensor.measure()
        print(sensor.temperature(), "C ", sensor.humidity(), "%")
    except Exception:           # OSError: no answer; Exception: bad checksum
        print("read failed")
    time.sleep(2)
