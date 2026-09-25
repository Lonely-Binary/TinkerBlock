/*
 * TK15-IR RECEIVER - ESP32-S3 Decoder Example
 *
 * Description:
 * This sketch decodes infrared remote signals with the TK15-IR RECEIVER
 * module. Unlike the simple "signal received" example, it identifies the
 * remote protocol (NEC, Sony, RC5, etc.) and prints the decoded address
 * and command for every button you press.
 *
 * Library:
 * Uses the "IRremote" library by Armin Joachimsmeyer (v4.x or newer).
 * Install it from Arduino IDE: Tools > Manage Libraries > search "IRremote".
 *
 * Wiring:
 * - VCC    -> ESP32-S3 3.3V
 * - GND    -> ESP32-S3 GND
 * - SIGNAL -> ESP32-S3 GPIO 3
 * - NC     -> Leave unconnected
 *
 * Usage:
 * 1. Install the IRremote library (see above)
 * 2. Upload this sketch to the ESP32-S3
 * 3. Open Serial Monitor (115200 baud)
 * 4. Point a remote at the module and press buttons to see decoded codes
 */

#include <IRremote.hpp>  // IRremote library (main header, include only once)

// Pin number: change this to match your wiring
#define IR_PIN 3             // ESP32-S3 GPIO connected to SIGNAL

// Map a decoded command code to its button name for this remote.
// Returns the button label, or "Unknown" if the code is not in the list.
// Edit these values to match your own remote's buttons.
const char* getButtonName(uint16_t command) {
  switch (command) {
    case 0x0C: return "Power On/Off";
    case 0x0F: return "Source";
    case 0x0D: return "Mute";
    case 0x42: return "Up";
    case 0x43: return "Down";
    case 0x44: return "Left";
    case 0x45: return "Right";
    case 0x46: return "Enter";
    case 0x78: return "Home";
    case 0x5B: return "Back";
    case 0x11: return "Option";
    case 0x14: return "Volume Up";
    case 0x15: return "Volume Down";
    case 0x9C: return "Search";
    case 0xB2: return "Red";
    case 0xB3: return "Green";
    case 0xB4: return "Blue";
    default:   return "Unknown";
  }
}

void setup() {
  // Start serial for output (115200 baud is standard for ESP32-S3)
  Serial.begin(115200);

  // Start the IR receiver. ENABLE_LED_FEEDBACK blinks the on-board LED
  // automatically whenever an IR signal is being received.
  IrReceiver.begin(IR_PIN);

  Serial.println("TK15-IR RECEIVER ready.");
  Serial.print("Listening for IR signals on GPIO ");
  Serial.println(IR_PIN);
  Serial.println("Point a remote at the module and press a button...");
}

void loop() {
  // decode() returns true when a complete IR frame has been received
  if (IrReceiver.decode()) {

    // Print a human-readable one-line summary (protocol, address, command, raw data)
    Serial.println("----------------------------------------");
    IrReceiver.printIRResultShort(&Serial);

    // Access the individual decoded fields for use in your own logic
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // Protocol not recognized: signal too weak, noise, or an unsupported remote
      Serial.println("Unknown protocol. Try again or use another remote.");
    } else {
      // Print the fields separately so they are easy to use in code
      Serial.print("Protocol: ");
      Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));

      Serial.print("Address : 0x");
      Serial.println(IrReceiver.decodedIRData.address, HEX);

      Serial.print("Command : 0x");
      Serial.println(IrReceiver.decodedIRData.command, HEX);

      // Look up the button name for this command code and print it
      Serial.print("Button  : ");
      Serial.println(getButtonName(IrReceiver.decodedIRData.command));

      // The "repeat" flag is set when a button is held down
      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        Serial.println("(button held - repeat)");
      }
    }

    // IMPORTANT: re-enable the receiver to catch the next IR frame
    IrReceiver.resume();
  }
}
