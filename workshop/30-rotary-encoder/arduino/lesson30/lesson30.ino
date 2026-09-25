// Pins: encoder CLK/DATA for direction, BTN for button, LED on PWM pin
#define CLK_PIN 4    // Encoder CLK on D4
#define DATA_PIN 5   // Encoder DATA on D5
#define BTN_PIN 3    // Encoder button BTN on D3
#define LED_PIN 6    // LED on D6 (PWM)

int brightness = 128;           // Current LED brightness (0-255)
int savedBrightness = 128;      // Stored when off; restore when on again
bool ledOn = true;             // LED state: true=on, false=off
int lastClkState = HIGH;       // Previous CLK for edge detection

void setup() {
  pinMode(CLK_PIN, INPUT_PULLUP);   // Encoder CLK, internal pull-up
  pinMode(DATA_PIN, INPUT_PULLUP);   // Encoder DATA
  pinMode(BTN_PIN, INPUT_PULLUP);    // Button: pressed = LOW
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);
  lastClkState = digitalRead(CLK_PIN);  // Initial CLK to avoid false edge
  analogWrite(LED_PIN, brightness);     // Start at brightness
}

void loop() {
  int clkState = digitalRead(CLK_PIN);
  
  // Rotation: CLK change = one step
  if (clkState != lastClkState) {
    if (clkState == LOW && ledOn) {
      int dataState = digitalRead(DATA_PIN);
      
      if (dataState == LOW) {
        brightness = min(255, brightness + 20);  // CW: brighter, cap 255
        Serial.println("CW - brightness up");
      } else {
        brightness = max(0, brightness - 20);   // CCW: dimmer, min 0
        Serial.println("CCW - brightness down");
      }
      
      analogWrite(LED_PIN, brightness);
    }
    lastClkState = clkState;
  }
  
  // Button: LOW when pressed. Toggle on/off; save brightness when off, restore when on
  if (digitalRead(BTN_PIN) == LOW) {
    ledOn = !ledOn;
    if (ledOn) {
      brightness = savedBrightness;
      analogWrite(LED_PIN, brightness);
      Serial.println("Button - LED on");
    } else {
      savedBrightness = brightness;  // Save before turning off
      brightness = 0;
      analogWrite(LED_PIN, 0);
      Serial.println("Button - LED off");
    }
    delay(200);   // Debounce
  }
  
  delay(10);   // Short delay for stable read
}
