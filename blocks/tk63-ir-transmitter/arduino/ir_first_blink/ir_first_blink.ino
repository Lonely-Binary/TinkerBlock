/*
  Infrared Transmitter - first blink                     TK63 / /p/tk63

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V (VBUS on a Pico) for full range; 3V3 works, shorter.
              VCC only feeds the two LEDs, so 5V is safe on any board.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 6 on an ESP32-S3, GPIO 22 on an ESP32,
              D3 on an Uno, GP17 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
    Serial Monitor                115200
*/

// The pin SIGNAL is wired to.
// Uno: 3. ESP32: 22. ESP32-S3: 6. Pico: 17.
const int IR_PIN = 6;

void setup() {
  // LOW first: nothing on the board holds the transistor off.
  pinMode(IR_PIN, OUTPUT);
  digitalWrite(IR_PIN, LOW);
  Serial.begin(115200);
}

void loop() {
  // HIGH turns the transistor on: both LEDs light.
  digitalWrite(IR_PIN, HIGH);
  Serial.println("on: the red LED is lit; look through a camera");
  delay(1000);

  digitalWrite(IR_PIN, LOW);
  Serial.println("off");
  delay(1000);
}
