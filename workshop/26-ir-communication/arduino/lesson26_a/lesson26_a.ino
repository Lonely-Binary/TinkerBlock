// Program 1: IR receiver (upload to board A with TK15 on D3)
#include <IRremote.hpp>

#define IR_RX_PIN 3   // TK15 SIGNAL on D3

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RX_PIN, DISABLE_LED_FEEDBACK);  // Start receiving
  Serial.println("IR receiver ready");
}

void loop() {
  if (IrReceiver.decode()) {                // A whole frame has arrived
    IRData &d = IrReceiver.decodedIRData;
    if (d.flags & IRDATA_FLAGS_IS_REPEAT) {
      Serial.println("[repeat]");           // A held button, no new data
    } else {
      Serial.print(getProtocolString(d.protocol));
      Serial.print("  address=0x");
      Serial.print(d.address, HEX);
      Serial.print("  command=0x");
      Serial.println(d.command, HEX);
    }
    IrReceiver.resume();                    // Ready for the next frame
  }
}
