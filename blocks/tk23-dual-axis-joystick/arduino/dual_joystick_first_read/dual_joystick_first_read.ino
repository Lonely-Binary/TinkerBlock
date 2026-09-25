/*
  Dual Axis Joystick - first read                        TK23 / /p/tk23

  Wiring. Parts up, header along the bottom. Count from the square pad,
  which is GND at the left-hand end, rightwards:

    GND -> GND
    VCC -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC  -> nothing   (in no net on the board)
    X   -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
           ESP32-S3, GP26 on a Raspberry Pi Pico      (analog)
    Y   -> A1, GPIO 35, GPIO 5, GP27      (same order, analog)
    SW  -> D2, GPIO 25, GPIO 7, GP15      (digital)

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pins X, Y and SW are wired to.
// Uno: A0 A1 2. ESP32: 34 35 25. ESP32-S3: 4 5 7. Pico: 26 27 15.
const int X_PIN = 4;
const int Y_PIN = 5;
const int SW_PIN = 7;

void setup() {
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT);         // R10 on the board pulls it down
}

void loop() {
  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);
  int sw = digitalRead(SW_PIN);   // HIGH while the stick is pressed

  Serial.print("X ");
  Serial.print(x);
  Serial.print("  Y ");
  Serial.print(y);
  Serial.print("  SW ");
  Serial.println(sw);
  delay(200);                     // five lines a second
}
