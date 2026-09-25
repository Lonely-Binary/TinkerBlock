/*
 * TK15-IR RECEIVER - ESP32-S3 Mac Controller (Bluetooth Keyboard)
 *
 * Description:
 * The ESP32-S3 receives infrared signals from a remote (via the TK15-IR
 * RECEIVER on GPIO 3) and re-sends them to a Mac as a Bluetooth (BLE)
 * keyboard + media remote. Point the remote at the module and it controls
 * your Mac from across the room.
 *
 * This sketch talks to the BLE HID stack DIRECTLY through NimBLE-Arduino
 * (v2.x). It does NOT use the "ESP32-BLE-Keyboard" library, because that
 * library only supports the old NimBLE 1.x API and will not compile on the
 * ESP32 core 3.x used here.
 *
 * Two kinds of key are sent:
 *   - Keyboard reports (Report ID 1) for arrows, Enter, Esc, shortcuts.
 *   - Consumer reports (Report ID 2) for media keys (volume, mute, play,
 *     next/prev) - this is how a Mac actually receives those keys.
 *
 * ----------------------------------------------------------------------
 * Required libraries (Arduino IDE > Tools > Manage Libraries):
 *   1. "IRremote"       by Armin Joachimsmeyer   (v4.x or newer)
 *   2. "NimBLE-Arduino" by h2zero                 (v2.x)
 * ----------------------------------------------------------------------
 *
 * Wiring:
 * - VCC    -> ESP32-S3 3.3V
 * - GND    -> ESP32-S3 GND
 * - SIGNAL -> ESP32-S3 GPIO 3
 * - NC     -> Leave unconnected
 *
 * Usage:
 * 1. Install the two libraries above and upload this sketch.
 * 2. On the Mac: System Settings > Bluetooth, pair "ESP32 IR Remote".
 * 3. Point the remote at the module and press buttons to control the Mac.
 *
 * IR button -> Mac action map:
 *   Power On/Off (0x0C) -> Lock screen        (Cmd+Ctrl+Q)
 *   Source       (0x0F) -> Switch app         (Cmd+Tab)
 *   Mute         (0x0D) -> Mute               (media)
 *   Up           (0x42) -> Arrow Up
 *   Down         (0x43) -> Arrow Down
 *   Left         (0x44) -> Arrow Left
 *   Right        (0x45) -> Arrow Right
 *   Enter        (0x46) -> Return
 *   Home         (0x78) -> Mission Control     (Ctrl+Up)
 *   Back         (0x5B) -> Escape
 *   Option       (0x11) -> Toggle Full Screen  (Cmd+Ctrl+F)
 *   Volume Up    (0x14) -> Volume Up           (media)
 *   Volume Down  (0x15) -> Volume Down         (media)
 *   Search       (0x9C) -> Spotlight           (Cmd+Space)
 *   Red          (0xB2) -> Play/Pause          (media)
 *   Green        (0xB3) -> Previous track      (media)
 *   Blue         (0xB4) -> Next track          (media)
 */

#include <IRremote.hpp>       // IRremote library (main header, include once)
#include <NimBLEDevice.h>     // NimBLE-Arduino v2.x
#include <NimBLEHIDDevice.h>

// Pin number: change this to match your wiring
#define IR_PIN 3              // ESP32-S3 GPIO connected to SIGNAL

// Serial debug: set to 1 for a detailed dump of every IR frame,
// or 0 for quiet operation (only connection + button lines).
#define DEBUG 1

// --- Samsung remote command codes (edit to match your remote) ---
#define CMD_POWER    0x0C
#define CMD_SOURCE   0x0F
#define CMD_MUTE     0x0D
#define CMD_UP       0x42
#define CMD_DOWN     0x43
#define CMD_LEFT     0x44
#define CMD_RIGHT    0x45
#define CMD_ENTER    0x46
#define CMD_HOME     0x78
#define CMD_BACK     0x5B
#define CMD_OPTION   0x11
#define CMD_VOL_UP   0x14
#define CMD_VOL_DOWN 0x15
#define CMD_SEARCH   0x9C
#define CMD_RED      0xB2
#define CMD_GREEN    0xB3
#define CMD_BLUE     0xB4

// --- Keyboard modifier bits (byte 0 of a keyboard report) ---
// On a Mac: GUI = Command, ALT = Option.
#define MOD_LCTRL  0x01
#define MOD_LSHIFT 0x02
#define MOD_LALT   0x04
#define MOD_LGUI   0x08   // Command key

// --- Keyboard HID usage codes (the actual key) ---
#define KC_F     0x09
#define KC_Q     0x14
#define KC_ENTER 0x28
#define KC_ESC   0x29
#define KC_TAB   0x2B
#define KC_SPACE 0x2C
#define KC_RIGHT 0x4F
#define KC_LEFT  0x50
#define KC_DOWN  0x51
#define KC_UP    0x52

// --- Consumer (media) usage codes, 16-bit ---
#define CC_PLAY_PAUSE 0x00CD
#define CC_NEXT       0x00B5
#define CC_PREV       0x00B6
#define CC_MUTE       0x00E2
#define CC_VOL_UP     0x00E9
#define CC_VOL_DOWN   0x00EA

// HID report map: one Keyboard collection (Report ID 1) and one
// Consumer Control collection (Report ID 2).
static uint8_t REPORT_MAP[] = {
  // ----- Keyboard (Report ID 1) -----
  0x05, 0x01,        // Usage Page (Generic Desktop)
  0x09, 0x06,        // Usage (Keyboard)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x05, 0x07,        //   Usage Page (Keyboard/Keypad)
  0x19, 0xE0,        //   Usage Minimum (Left Control)
  0x29, 0xE7,        //   Usage Maximum (Right GUI)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x81, 0x02,        //   Input (Data,Var,Abs)  ; modifier byte
  0x95, 0x01,        //   Report Count (1)
  0x75, 0x08,        //   Report Size (8)
  0x81, 0x01,        //   Input (Const)         ; reserved byte
  0x95, 0x06,        //   Report Count (6)
  0x75, 0x08,        //   Report Size (8)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x65,        //   Logical Maximum (101)
  0x05, 0x07,        //   Usage Page (Keyboard/Keypad)
  0x19, 0x00,        //   Usage Minimum (0)
  0x29, 0x65,        //   Usage Maximum (101)
  0x81, 0x00,        //   Input (Data,Array)    ; up to 6 keys
  0xC0,              // End Collection

  // ----- Consumer Control (Report ID 2) -----
  0x05, 0x0C,        // Usage Page (Consumer)
  0x09, 0x01,        // Usage (Consumer Control)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x02,        //   Report ID (2)
  0x15, 0x00,        //   Logical Minimum (0)
  0x26, 0xFF, 0x03,  //   Logical Maximum (0x3FF)
  0x19, 0x00,        //   Usage Minimum (0)
  0x2A, 0xFF, 0x03,  //   Usage Maximum (0x3FF)
  0x75, 0x10,        //   Report Size (16)
  0x95, 0x01,        //   Report Count (1)
  0x81, 0x00,        //   Input (Data,Array)
  0xC0               // End Collection
};

NimBLEHIDDevice*      hid            = nullptr;
NimBLECharacteristic* inputKeyboard  = nullptr;  // Report ID 1
NimBLECharacteristic* inputConsumer  = nullptr;  // Report ID 2
volatile bool         deviceConnected = false;

// Track connect/disconnect and keep advertising when idle
class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    deviceConnected = true;
  }
  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
    deviceConnected = false;
    NimBLEDevice::getAdvertising()->start();  // allow re-connection
  }
};

// Human-readable name for logging, or "Unknown" if not mapped.
const char* getButtonName(uint16_t command) {
  switch (command) {
    case CMD_POWER:    return "Power On/Off";
    case CMD_SOURCE:   return "Source";
    case CMD_MUTE:     return "Mute";
    case CMD_UP:       return "Up";
    case CMD_DOWN:     return "Down";
    case CMD_LEFT:     return "Left";
    case CMD_RIGHT:    return "Right";
    case CMD_ENTER:    return "Enter";
    case CMD_HOME:     return "Home";
    case CMD_BACK:     return "Back";
    case CMD_OPTION:   return "Option";
    case CMD_VOL_UP:   return "Volume Up";
    case CMD_VOL_DOWN: return "Volume Down";
    case CMD_SEARCH:   return "Search";
    case CMD_RED:      return "Red";
    case CMD_GREEN:    return "Green";
    case CMD_BLUE:     return "Blue";
    default:           return "Unknown";
  }
}

// Human-readable description of the Mac action a command triggers.
const char* getActionName(uint16_t command) {
  switch (command) {
    case CMD_POWER:    return "Lock screen (Cmd+Ctrl+Q)";
    case CMD_SOURCE:   return "Switch app (Cmd+Tab)";
    case CMD_MUTE:     return "Mute (media)";
    case CMD_UP:       return "Arrow Up";
    case CMD_DOWN:     return "Arrow Down";
    case CMD_LEFT:     return "Arrow Left";
    case CMD_RIGHT:    return "Arrow Right";
    case CMD_ENTER:    return "Return";
    case CMD_HOME:     return "Mission Control (Ctrl+Up)";
    case CMD_BACK:     return "Escape";
    case CMD_OPTION:   return "Full Screen (Cmd+Ctrl+F)";
    case CMD_VOL_UP:   return "Volume Up (media)";
    case CMD_VOL_DOWN: return "Volume Down (media)";
    case CMD_SEARCH:   return "Spotlight (Cmd+Space)";
    case CMD_RED:      return "Play/Pause (media)";
    case CMD_GREEN:    return "Previous track (media)";
    case CMD_BLUE:     return "Next track (media)";
    default:           return "(no action mapped)";
  }
}

// Send one keyboard keypress (with optional modifiers), then release.
void sendKey(uint8_t modifiers, uint8_t keycode) {
  uint8_t press[8]   = { modifiers, 0, keycode, 0, 0, 0, 0, 0 };
  uint8_t release[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  inputKeyboard->setValue(press, sizeof(press));
  inputKeyboard->notify();
  delay(15);
  inputKeyboard->setValue(release, sizeof(release));
  inputKeyboard->notify();
}

// Send one consumer/media key, then release.
void sendConsumer(uint16_t usage) {
  uint8_t press[2]   = { (uint8_t)(usage & 0xFF), (uint8_t)(usage >> 8) };
  uint8_t release[2] = { 0, 0 };
  inputConsumer->setValue(press, sizeof(press));
  inputConsumer->notify();
  delay(15);
  inputConsumer->setValue(release, sizeof(release));
  inputConsumer->notify();
}

// Translate one IR command into a Mac keyboard/media action.
void handleButton(uint16_t command) {
  switch (command) {
    case CMD_POWER:    sendKey(MOD_LCTRL | MOD_LGUI, KC_Q);  break;  // Lock screen
    case CMD_SOURCE:   sendKey(MOD_LGUI, KC_TAB);            break;  // Switch app
    case CMD_MUTE:     sendConsumer(CC_MUTE);                break;
    case CMD_UP:       sendKey(0, KC_UP);                    break;
    case CMD_DOWN:     sendKey(0, KC_DOWN);                  break;
    case CMD_LEFT:     sendKey(0, KC_LEFT);                  break;
    case CMD_RIGHT:    sendKey(0, KC_RIGHT);                 break;
    case CMD_ENTER:    sendKey(0, KC_ENTER);                 break;
    case CMD_HOME:     sendKey(MOD_LCTRL, KC_UP);            break;  // Mission Control
    case CMD_BACK:     sendKey(0, KC_ESC);                   break;
    case CMD_OPTION:   sendKey(MOD_LCTRL | MOD_LGUI, KC_F);  break;  // Full screen
    case CMD_VOL_UP:   sendConsumer(CC_VOL_UP);              break;
    case CMD_VOL_DOWN: sendConsumer(CC_VOL_DOWN);            break;
    case CMD_SEARCH:   sendKey(MOD_LGUI, KC_SPACE);          break;  // Spotlight
    case CMD_RED:      sendConsumer(CC_PLAY_PAUSE);          break;
    case CMD_GREEN:    sendConsumer(CC_PREV);                break;
    case CMD_BLUE:     sendConsumer(CC_NEXT);                break;
    default: /* unmapped code: do nothing */ break;
  }
}

// Buttons allowed to keep firing while the remote button is held down.
bool allowsRepeat(uint16_t command) {
  return command == CMD_UP   || command == CMD_DOWN ||
         command == CMD_LEFT || command == CMD_RIGHT ||
         command == CMD_VOL_UP || command == CMD_VOL_DOWN;
}

void setup() {
  Serial.begin(115200);

  // ----- Start the IR receiver -----
  IrReceiver.begin(IR_PIN);

  // ----- Start BLE as an HID keyboard -----
  NimBLEDevice::init("ESP32 IR Remote");
  NimBLEDevice::setSecurityAuth(true, false, true);  // bonding + secure connections

  NimBLEServer* server = NimBLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  hid = new NimBLEHIDDevice(server);
  hid->setManufacturer("TinkerBlock");
  hid->setPnp(0x02, 0xE502, 0xA111, 0x0210);
  hid->setHidInfo(0x00, 0x01);

  inputKeyboard = hid->getInputReport(1);   // Report ID 1 (keyboard)
  inputConsumer = hid->getInputReport(2);   // Report ID 2 (consumer/media)

  hid->setReportMap(REPORT_MAP, sizeof(REPORT_MAP));
  hid->setBatteryLevel(100);

  server->start();

  // ----- Advertise as a keyboard -----
  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->setAppearance(HID_KEYBOARD);                     // 0x03C1
  adv->addServiceUUID(hid->getHidService()->getUUID());
  adv->enableScanResponse(true);
  adv->start();

  Serial.println("TK15-IR Mac Controller starting...");
  Serial.print("Listening for IR on GPIO ");
  Serial.println(IR_PIN);
  Serial.println("Pair 'ESP32 IR Remote' on your Mac (System Settings > Bluetooth).");
}

void loop() {
  // Report BLE connection state changes once
  static bool wasConnected = false;
  if (deviceConnected != wasConnected) {
    Serial.println(deviceConnected ? "[BLE] Mac connected."
                                   : "[BLE] Mac disconnected - waiting to pair...");
    wasConnected = deviceConnected;
  }

  // A complete IR frame is ready
  if (IrReceiver.decode()) {
    uint16_t     command  = IrReceiver.decodedIRData.command;
    bool         isRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
    decode_type_t protocol = IrReceiver.decodedIRData.protocol;

#if DEBUG
    // Full dump of every decoded frame - handy for adding new remotes
    Serial.println("---------------- IR frame ----------------");
    Serial.print("Protocol : "); Serial.println(getProtocolString(protocol));
    Serial.print("Address  : 0x"); Serial.println(IrReceiver.decodedIRData.address, HEX);
    Serial.print("Command  : 0x"); Serial.println(command, HEX);
    Serial.print("Raw-Data : 0x"); Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    Serial.print("Repeat   : "); Serial.println(isRepeat ? "yes" : "no");
#endif

    // Only act on our Samsung remote (ignore noise / other protocols)
    if (protocol == SAMSUNG) {

      // Fire on a fresh press; on a held-down repeat, only for repeatable keys
      if (!isRepeat || allowsRepeat(command)) {
        const char* button = getButtonName(command);
        const char* action = getActionName(command);

#if DEBUG
        Serial.print("Button   : "); Serial.println(button);
        Serial.print("Action   : "); Serial.println(action);
#else
        Serial.print("Button: "); Serial.print(button);
        if (isRepeat) Serial.print(" (repeat)");
        Serial.println();
#endif

        if (deviceConnected) {
          handleButton(command);            // send the keystroke to the Mac
#if DEBUG
          Serial.println("BLE      : sent to Mac");
#endif
        } else {
          Serial.println("BLE      : NOT sent (Mac not connected)");
        }
      } else {
#if DEBUG
        Serial.println("Button   : (held repeat - ignored for this key)");
#endif
      }
    } else {
#if DEBUG
      Serial.println("Ignored  : not a Samsung remote");
#endif
    }

    IrReceiver.resume();               // ready for the next IR frame
  }
}
