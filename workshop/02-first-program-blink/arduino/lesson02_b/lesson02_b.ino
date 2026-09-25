// First program: LED on D13 on 1 s, off 1 s, repeat
void setup() {
  pinMode(13, OUTPUT);   // set pin 13 as output so digitalWrite can control the LED
}

void loop() {
  digitalWrite(13, HIGH);   // pin 13 high, TK01 LED on
  delay(1000);              // hold 1000 ms (1 s)
  digitalWrite(13, LOW);    // pin 13 low, LED off
  delay(1000);              // wait 1 s then back to start of loop, so it repeats
}
