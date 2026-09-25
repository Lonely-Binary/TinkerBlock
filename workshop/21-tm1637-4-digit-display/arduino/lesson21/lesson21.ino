#include <TM1637Display.h>   // Include library

#define CLK_PIN 2   // CLK on D2
#define DIO_PIN 3   // DIO on D3

// Create display object
TM1637Display display(CLK_PIN, DIO_PIN);

void setup() {
  display.setBrightness(7);   // Brightness 0-7, 7 = brightest
  Serial.begin(9600);
  Serial.println("4-digit display program started");
}

void loop() {
  // Show 0-9999 in a loop
  for (int i = 0; i < 10000; i++) {
    display.showNumberDec(i);   // Show number i
    Serial.print("Display: ");
    Serial.println(i);
    delay(500);
  }
}
