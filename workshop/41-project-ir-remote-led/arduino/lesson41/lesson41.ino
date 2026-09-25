// IR remote LED: one button = on, another = off; print each command
#include <IRremote.hpp>
#define IR_PIN 3    // TK15 SIGNAL on D3
#define LED_PIN 13  // TK01 SIGNAL on D13

// REPLACE both with commands your own remote prints (see step 5)
#define ON_COMMAND  0x16
#define OFF_COMMAND 0x19

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN, DISABLE_LED_FEEDBACK);   // Start receiving
  Serial.println("IR remote LED started");
}

void loop() {
  if (IrReceiver.decode()) {                // A whole frame has arrived
    IRData &d = IrReceiver.decodedIRData;

    if (!(d.flags & IRDATA_FLAGS_IS_REPEAT)) {  // Ignore a held button's repeats
      Serial.print("command=0x");
      Serial.println(d.command, HEX);

      if (d.command == ON_COMMAND) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED on");
      } else if (d.command == OFF_COMMAND) {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED off");
      }
    }

    IrReceiver.resume();                    // Ready for the next frame
  }
}
