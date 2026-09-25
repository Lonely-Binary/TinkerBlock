/*
 * TK16-IR REMOTE SENDER - ESP32-S3 IR Clone (Copy & Replay)
 *
 * Description:
 * A serial-menu tool that CLONES an infrared remote. Point an original remote
 * at the IR receiver and "copy" a button into a memory slot, then "send" that
 * slot through the IR sender to reproduce the exact same signal - turning the
 * ESP32-S3 into a universal remote.
 *
 * It clones both well-known protocols (NEC, Samsung, Sony, RC5, ...) by storing
 * the decoded address/command, and unknown/long signals by storing the raw
 * timing and replaying it. Up to 5 buttons can be stored at once.
 *
 * Hardware (two separate modules):
 * - TK15 IR RECEIVER  SIGNAL -> ESP32-S3 GPIO 3
 * - TK16 IR SENDER    SIGNAL -> ESP32-S3 GPIO 4
 *   (both modules: VCC -> 3.3V, GND -> GND)
 *
 * Library:
 * Uses the "IRremote" library by Armin Joachimsmeyer (v4.x or newer).
 * Install it from Arduino IDE: Tools > Manage Libraries > search "IRremote".
 *
 * Usage:
 * 1. Upload this sketch to the ESP32-S3.
 * 2. Open Serial Monitor at 115200 baud, line ending = "Newline".
 * 3. Follow the menu:
 *      c <n>  copy the next remote press into slot n
 *      s <n>  send (replay) slot n through the IR sender
 *      l      list stored slots
 *      e <n>  erase slot n
 *      h      show the menu
 */

// RAW_BUFFER_LENGTH must be defined BEFORE including IRremote. 750 is large
// enough for most remotes, including many air-conditioner codes.
#define RAW_BUFFER_LENGTH 750

#include <IRremote.hpp>   // IRremote library (main header, include only once)

// Pin numbers: change these to match your wiring
#define IR_RECEIVE_PIN 3  // TK15 receiver SIGNAL
#define IR_SEND_PIN    4  // TK16 sender SIGNAL

#define NUM_SLOTS 5       // how many buttons can be stored at once

// One stored button. Known protocols use "data"; unknown/long signals use "raw".
struct IRSlot {
  bool     used = false;
  bool     isRaw = false;
  IRData   data;                     // decoded protocol/address/command (known codes)
  uint8_t  rawCode[RAW_BUFFER_LENGTH]; // timing ticks (raw / unknown codes)
  uint16_t rawLen = 0;
};

IRSlot slots[NUM_SLOTS];

void printMenu() {
  Serial.println();
  Serial.println(F("============== IR CLONE =============="));
  Serial.println(F("  c <n>   Copy : capture next remote press into slot n"));
  Serial.println(F("  s <n>   Send : replay slot n through the IR sender"));
  Serial.println(F("  l       List : show all stored slots"));
  Serial.println(F("  e <n>   Erase: clear slot n"));
  Serial.println(F("  h       Help : show this menu"));
  Serial.print(F("  n = 1.."));
  Serial.println(NUM_SLOTS);
  Serial.println(F("  (Set Serial Monitor line ending to \"Newline\")"));
  Serial.println(F("====================================="));
}

// Print a one-line summary of a slot.
void printSlot(int idx) {
  IRSlot &sl = slots[idx];
  Serial.print(F("  Slot "));
  Serial.print(idx + 1);
  Serial.print(F(": "));
  if (!sl.used) {
    Serial.println(F("(empty)"));
    return;
  }
  if (sl.isRaw) {
    Serial.print(F("RAW  ("));
    Serial.print(sl.rawLen);
    Serial.println(F(" timings)"));
  } else {
    Serial.print(getProtocolString(sl.data.protocol));
    Serial.print(F("  Address=0x"));
    Serial.print(sl.data.address, HEX);
    Serial.print(F(" Command=0x"));
    Serial.print(sl.data.command, HEX);
    Serial.print(F(" Bits="));
    Serial.println(sl.data.numberOfBits);
  }
}

void listSlots() {
  Serial.println(F("Stored slots:"));
  for (int i = 0; i < NUM_SLOTS; i++) {
    printSlot(i);
  }
}

// Copy the currently decoded IR frame into slot idx.
void storeToSlot(int idx) {
  IRSlot &sl = slots[idx];
  sl.data = IrReceiver.decodedIRData;

  decode_type_t p = sl.data.protocol;
  if (p == UNKNOWN || p == PULSE_WIDTH || p == PULSE_DISTANCE) {
    // Not a recognised protocol -> store the raw timing for replay
    sl.isRaw  = true;
    sl.rawLen = IrReceiver.irparams.rawlen - 1;
    IrReceiver.compensateAndStoreIRResultInArray(sl.rawCode);
  } else {
    // Recognised protocol -> store decoded fields, clear repeat flag
    sl.isRaw = false;
    sl.data.flags = 0;
  }
  sl.used = true;

  Serial.print(F("Copied into slot "));
  Serial.println(idx + 1);
  printSlot(idx);
}

// Wait for the next remote press and copy it into slot idx.
void captureToSlot(int idx) {
  Serial.print(F("Point the ORIGINAL remote at the receiver and press the button for slot "));
  Serial.println(idx + 1);
  Serial.println(F("(waiting up to 15s - send any key to cancel)"));

  // Drain the leftover line-ending from the command (e.g. the '\n' after
  // "c 1\r") so it is not mistaken for a cancel keypress. The short delay
  // lets a trailing terminator finish arriving before we flush it.
  delay(20);
  while (Serial.available()) Serial.read();

  IrReceiver.resume();   // discard any stale frame; wait for a fresh press
  unsigned long start = millis();

  while (millis() - start < 15000) {
    // A real keypress cancels the capture (ignore stray CR/LF/space)
    if (Serial.available()) {
      char c = Serial.read();
      if (c != '\r' && c != '\n' && c != ' ') {
        while (Serial.available()) Serial.read();
        Serial.println(F("Capture cancelled."));
        return;
      }
    }

    if (IrReceiver.decode()) {
      uint8_t f = IrReceiver.decodedIRData.flags;

      // Ignore held-button repeats - wait for the initial full frame
      if (f & (IRDATA_FLAGS_IS_REPEAT | IRDATA_FLAGS_IS_AUTO_REPEAT)) {
        IrReceiver.resume();
        continue;
      }
      // Signal too long for the raw buffer
      if (f & IRDATA_FLAGS_WAS_OVERFLOW) {
        Serial.println(F("Signal too long (overflow) - increase RAW_BUFFER_LENGTH."));
        IrReceiver.resume();
        continue;
      }
      // Too short to be real
      if (IrReceiver.irparams.rawlen < 4) {
        IrReceiver.resume();
        continue;
      }

      storeToSlot(idx);
      IrReceiver.resume();
      return;
    }

    delay(2);   // yield to FreeRTOS so the busy-wait can't starve the scheduler
  }
  Serial.println(F("Timed out - no IR signal captured."));
}

// Replay slot idx through the IR sender.
void sendSlot(int idx) {
  IRSlot &sl = slots[idx];
  if (!sl.used) {
    Serial.print(F("Slot "));
    Serial.print(idx + 1);
    Serial.println(F(" is empty - copy something first."));
    return;
  }

  Serial.print(F("Sending slot "));
  Serial.println(idx + 1);
  Serial.flush();   // keep serial quiet while generating the carrier

  // IMPORTANT on ESP32: stop the 50us receive ISR before transmitting.
  // The sender uses the hardware PWM (LEDC) carrier; leaving the receiver
  // timer running during a send can corrupt the FreeRTOS scheduler and crash.
  IrReceiver.stop();

  if (sl.isRaw) {
    IrSender.sendRaw(sl.rawCode, sl.rawLen, 38);  // assume 38 kHz carrier
  } else {
    IrSender.write(&sl.data, 0);                  // 0 extra repeats
  }

  IrReceiver.start();              // re-enable receiving after transmitting
  Serial.println(F("Sent."));
}

void eraseSlot(int idx) {
  slots[idx].used  = false;
  slots[idx].isRaw = false;
  slots[idx].rawLen = 0;
  Serial.print(F("Erased slot "));
  Serial.println(idx + 1);
}

// Parse one command line, e.g. "c 1", "s3", "l".
void handleCommand(const String &line) {
  char cmd = tolower(line[0]);

  // Find the first digit anywhere in the line -> slot number
  int slot = -1;
  for (uint16_t i = 1; i < line.length(); i++) {
    if (isdigit(line[i])) {
      slot = line[i] - '0';
      break;
    }
  }

  bool needsSlot = (cmd == 'c' || cmd == 's' || cmd == 'e');
  if (needsSlot && slot == -1) slot = 1;  // default to slot 1 if omitted

  if (needsSlot && (slot < 1 || slot > NUM_SLOTS)) {
    Serial.print(F("Slot must be 1.."));
    Serial.println(NUM_SLOTS);
    return;
  }

  switch (cmd) {
    case 'c': captureToSlot(slot - 1); break;
    case 's': sendSlot(slot - 1);      break;
    case 'e': eraseSlot(slot - 1);     break;
    case 'l': listSlots();             break;
    case 'h':
    case '?': printMenu();             break;
    default:
      Serial.println(F("Unknown command. Type 'h' for help."));
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Start receiver and sender
  IrReceiver.begin(IR_RECEIVE_PIN);
  IrSender.begin(IR_SEND_PIN);

  Serial.println(F("TK16 IR Clone ready."));
  Serial.print(F("Receiver on GPIO "));
  Serial.print(IR_RECEIVE_PIN);
  Serial.print(F(", Sender on GPIO "));
  Serial.println(IR_SEND_PIN);
  printMenu();
}

void loop() {
  // Read serial commands line by line
  static String inputBuffer = "";
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        handleCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}
