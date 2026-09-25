// Pin number: change these to match your wiring
#define LATCH_PIN 8   // Arduino digital pin connected to LATCH (e.g. D8)
#define CLOCK_PIN 12  // Arduino digital pin connected to CLOCK (e.g. D12)
#define DATA_PIN 11   // Arduino digital pin connected to DATA (e.g. D11)

// 7-segment display segment code table (common anode, numbers 0-9)
byte digitPatterns[10] = {
  0b11111100,  // 0
  0b01100000,  // 1
  0b11011010,  // 2
  0b11110010,  // 3
  0b01100110,  // 4
  0b10110110,  // 5
  0b10111110,  // 6
  0b11100000,  // 7
  0b11111110,  // 8
  0b11110110   // 9
};

void setup() {
  // Initialize pin modes
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  
  // Start serial for debugging (9600 baud)
  Serial.begin(9600);
  
  Serial.println("74HC595 7-segment display program started");
  Serial.println("Cycling through 0-9");
}

void loop() {
  // Cycle through 0-9
  for (int i = 0; i < 10; i++) {
    displayDigit(i);
    Serial.print("Display number: ");
    Serial.println(i);
    delay(1000);  // Switch number every second
  }
}

// Display digit function
void displayDigit(int digit) {
  if (digit < 0 || digit > 9) return;  // Check range
  
  // Latch pin LOW, ready to receive data
  digitalWrite(LATCH_PIN, LOW);
  
  // Send data through shift register
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitPatterns[digit]);
  
  // Latch pin HIGH, output data to display
  digitalWrite(LATCH_PIN, HIGH);
}
