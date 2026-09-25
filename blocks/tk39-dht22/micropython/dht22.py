import dht, machine, time

# ESP32: 18. ESP32-S3: 4. Pico: 2.
sensor = dht.DHT22(machine.Pin(4))

while True:
    time.sleep(2)   # more than 2 s between readings
    try:
        sensor.measure()
    except Exception as e:
        # OSError if nothing answered, Exception for a bad checksum.
        print("read failed:", e)
        continue
    print("{:.1f} C  {:.1f} %".format(sensor.temperature(), sensor.humidity()))
