/*
  74HC595 Segment LED - count 0 to 9                  TK53 / /p/tk53

  Wiring. Count from pin 1, printed GND-1 on the TinkerBlock board
  (this board has no square pad), digit up, header at the bottom:

    GND   -> GND
    VCC   -> your board's logic supply: 5V on an Uno, 3V3 on an
             ESP32, ESP32-S3 or Pico. At 5V the chip may not read
             a 3.3 V board's HIGH, so never 5V beside a 3.3 V board.
    NC    -> nothing   (unconnected on the board)
    LATCH -> D8 on an Uno, GPIO 27 on an ESP32, GPIO 6 on an
             ESP32-S3, GP2 on a Raspberry Pi Pico
    CLOCK -> D12 on an Uno, GPIO 26 on an ESP32, GPIO 5 on an
             ESP32-S3, GP3 on a Raspberry Pi Pico
    DATA  -> D11 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
             ESP32-S3, GP4 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board            your board, e.g. Arduino Uno
    Tools > Port             the one that appears when you plug in
    Tools > USB CDC On Boot  Enabled   (ESP32-S3 only)
    No library to install: shiftOut is built in.
    Serial Monitor at 115200.
*/

// The pins LATCH, CLOCK and DATA are wired to. These are the Uno's.
// ESP32: 27, 26, 25. ESP32-S3: 6, 5, 4. Pico: 2, 3, 4.
const int LATCH_PIN = 8;
const int CLOCK_PIN = 12;
const int DATA_PIN = 11;

// One byte per digit. Bit 0 is segment a (QA) and bit 7 the
// decimal point (QH). The digit is common cathode: a 1 lights.
const byte DIGITS[10] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111,  // 9
};
const byte DOT = 0b10000000;   // the decimal point, QH

// Eight bits in, then one latch: the digit changes all at once.
void show(byte pattern) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
  digitalWrite(LATCH_PIN, HIGH);         // the rising edge shows it
}

void setup() {
  Serial.begin(115200);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  show(0);                               // blank
}

void loop() {
  for (int n = 0; n < 10; n++) {
    show(DIGITS[n]);
    Serial.print(n);
    Serial.print("  0x");
    Serial.println(DIGITS[n], HEX);
    delay(1000);
  }
  show(DOT);                             // a dot between rounds
  delay(1000);
}
