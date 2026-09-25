/*
  8x8 LED Matrix - the first picture                 TK52 / /p/tk52

  Wiring. Matrix up, header at the bottom, the pins read
  GND VCC CS DIN NC CLK from the left. There is no square pad
  on this board: count from GND, printed on both sides.

    GND -> GND
    VCC -> 5V (VBUS on a Pico). The MAX7219 needs 4.0 to 5.5 V,
           so never 3V3.
    CS  -> D10 on an Uno, GPIO 5 on an ESP32, GPIO 10 on an
           ESP32-S3, GP17 on a Raspberry Pi Pico
    DIN -> D11 on an Uno, GPIO 23 on an ESP32, GPIO 11 on an
           ESP32-S3, GP19 on a Raspberry Pi Pico
    NC  -> nothing: no trace on the board reaches it
    CLK -> D13 on an Uno, GPIO 18 on an ESP32, GPIO 12 on an
           ESP32-S3, GP18 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board            your board, e.g. Arduino Uno
    Tools > Port             the one that appears when you plug in
    Tools > USB CDC On Boot  Enabled   (ESP32-S3 only)
    No library to install: shiftOut comes with every board.
    Serial Monitor at 115200.
*/

// Uno pins. ESP32: 5, 23, 18. ESP32-S3: 10, 11, 12. Pico: 17, 19, 18.
const int CS_PIN  = 10;
const int DIN_PIN = 11;
const int CLK_PIN = 13;

// If the picture comes out on its side, try 1, 2 or 3 quarter
// turns. If it comes out mirrored, set FLIP to true.
const int  TURN = 0;
const bool FLIP = false;

// Pictures: the top row first, the leftmost pixel in the top bit.
const uint8_t HEART[8] = {
  0b00000000, 0b01100110, 0b11111111, 0b11111111,
  0b11111111, 0b01111110, 0b00111100, 0b00011000,
};
const uint8_t ARROW[8] = {
  0b00011000, 0b00111100, 0b01111110, 0b11011011,
  0b00011000, 0b00011000, 0b00011000, 0b00011000,
};

// One 16-bit message: the register's address, then its data,
// top bit first. CS going back HIGH is what makes it take effect.
void send(uint8_t reg, uint8_t data) {
  digitalWrite(CS_PIN, LOW);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, reg);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  digitalWrite(CS_PIN, HIGH);
}

// Is the picture's pixel at (x, y) lit? x from the left, y down.
bool lit(const uint8_t *pic, int x, int y) {
  if (FLIP) x = 7 - x;
  for (int t = 0; t < TURN; t++) {
    int s = x;
    x = y;
    y = 7 - s;
  }
  return pic[y] & (0x80 >> x);
}

// On the TK52, register d + 1 lights column d from the left, and
// bit b of it lights the pixel b rows down from the top.
void show(const uint8_t *pic) {
  for (int d = 0; d < 8; d++) {
    uint8_t column = 0;
    for (int b = 0; b < 8; b++) {
      if (lit(pic, d, b)) column |= 1 << b;
    }
    send(1 + d, column);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(CS_PIN, OUTPUT);
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  send(0x0F, 0);  // display test off
  send(0x09, 0);  // no decoding: every bit is one LED
  send(0x0B, 7);  // scan all eight columns, never fewer
  send(0x0A, 4);  // brightness 4 of 15
  send(0x0C, 1);  // wake up: it powers up in shutdown
}

void loop() {
  show(HEART);
  Serial.println("heart");
  delay(1000);
  show(ARROW);
  Serial.println("arrow");
  delay(1000);
}
