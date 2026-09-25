// Pin number: change this to match your wiring
#define IR_TX_PIN 3  // Arduino digital pin connected to SIGNAL (e.g. D3, recommended to use PWM-capable pin)

void setup() {
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
  
  Serial.println("Infrared transmitter program started");
  Serial.println("Transmitting infrared signal...");
}

void loop() {
  // Transmit infrared signal (38kHz modulation)
  // Note: Arduino Uno can use tone() function to generate 38kHz frequency
  // But IRremote library is more recommended for infrared communication
  
  // Simple example: rapid switching to simulate transmission (IRremote library recommended for actual applications)
  for (int i = 0; i < 100; i++) {
    digitalWrite(IR_TX_PIN, HIGH);
    delayMicroseconds(13);  // 38kHz period is about 26 microseconds, HIGH for 13 microseconds
    digitalWrite(IR_TX_PIN, LOW);
    delayMicroseconds(13);
  }
  
  Serial.println("Infrared signal transmitted");
  delay(1000);  // Wait 1 second before transmitting again
}
