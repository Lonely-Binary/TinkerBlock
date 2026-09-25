/*
 * TK01-XL LED - Morse Code Demo (Arduino Uno R3)
 *
 * Description:
 * Uses LED on/off to display Morse code. Edit MESSAGE below; the LED will flash
 * that string in a loop with a 2-second pause between repeats. Supports A-Z and 0-9. Space = pause between words.
 *
 * Wiring:
 * Change LED_PIN below to match your wiring.
 *
 * Usage:
 * 1. Set MESSAGE to your text (e.g. "TK01" or "HELLO")
 * 2. Upload to Arduino Uno R3
 * 3. Open Serial Monitor (9600) to see the character being sent
 */

#define LED_PIN 3  // Connect to TK01 SIGNAL; change to match your wiring

// ----- Edit this string (A-Z, 0-9, space). LED will send it in Morse in a loop. -----
const char MESSAGE[] = "SOS";

// Morse code patterns: . = dot, - = dash (A-Z, 0-9)
#define MORSE_A ".-"
#define MORSE_B "-..."
#define MORSE_C "-.-."
#define MORSE_D "-.."
#define MORSE_E "."
#define MORSE_F "..-."
#define MORSE_G "--."
#define MORSE_H "...."
#define MORSE_I ".."
#define MORSE_J ".---"
#define MORSE_K "-.-"
#define MORSE_L ".-.."
#define MORSE_M "--"
#define MORSE_N "-."
#define MORSE_O "---"
#define MORSE_P ".--."
#define MORSE_Q "--.-"
#define MORSE_R ".-."
#define MORSE_S "..."
#define MORSE_T "-"
#define MORSE_U "..-"
#define MORSE_V "...-"
#define MORSE_W ".--"
#define MORSE_X "-..-"
#define MORSE_Y "-.--"
#define MORSE_Z "--.."
#define MORSE_0 "-----"
#define MORSE_1 ".----"
#define MORSE_2 "..---"
#define MORSE_3 "...--"
#define MORSE_4 "....-"
#define MORSE_5 "....."
#define MORSE_6 "-...."
#define MORSE_7 "--..."
#define MORSE_8 "---.."
#define MORSE_9 "----."

// Time units (ms): 1 dot = 1 unit, 1 dash = 3 units
const int DOT_MS = 200;             // Dot (short) duration
const int DASH_MS = DOT_MS * 3;     // Dash (long) duration
const int ELEMENT_GAP_MS = DOT_MS;  // Gap between dot/dash in same letter
const int LETTER_GAP_MS = DOT_MS * 3; // Gap between letters
const int WORD_GAP_MS = DOT_MS * 7;  // Gap for space (between words)

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Morse - loop, 2s pause between repeats");
}

void loop() {
  // Send each character of MESSAGE in Morse
  for (unsigned int i = 0; i < sizeof(MESSAGE) - 1; i++) {
    char c = MESSAGE[i];
    if (c == ' ') {
      delay(WORD_GAP_MS);
      Serial.println(" [space]");
    } else {
      sendMorseChar(c);
      delay(LETTER_GAP_MS);
    }
  }
  Serial.println("Done");
  delay(2000);  // Pause 2 seconds before next repeat
}

void ledOn(int ms) {
  digitalWrite(LED_PIN, HIGH);
  delay(ms);
}

void ledOff(int ms) {
  digitalWrite(LED_PIN, LOW);
  delay(ms);
}

void sendDot() {
  ledOn(DOT_MS);
  ledOff(ELEMENT_GAP_MS);
}

void sendDash() {
  ledOn(DASH_MS);
  ledOff(ELEMENT_GAP_MS);
}

// Send a pattern string (e.g. ".-" for A)
void sendMorsePattern(const char* pattern) {
  for (int i = 0; pattern[i] != '\0'; i++) {
    if (pattern[i] == '.') sendDot();
    else if (pattern[i] == '-') sendDash();
  }
}

void sendMorseChar(char c) {
  Serial.print("Sending: ");
  Serial.println(c);

  if (c >= 'a' && c <= 'z') c = c - 32;  // to upper
  switch (c) {
    case 'A': sendMorsePattern(MORSE_A); break;
    case 'B': sendMorsePattern(MORSE_B); break;
    case 'C': sendMorsePattern(MORSE_C); break;
    case 'D': sendMorsePattern(MORSE_D); break;
    case 'E': sendMorsePattern(MORSE_E); break;
    case 'F': sendMorsePattern(MORSE_F); break;
    case 'G': sendMorsePattern(MORSE_G); break;
    case 'H': sendMorsePattern(MORSE_H); break;
    case 'I': sendMorsePattern(MORSE_I); break;
    case 'J': sendMorsePattern(MORSE_J); break;
    case 'K': sendMorsePattern(MORSE_K); break;
    case 'L': sendMorsePattern(MORSE_L); break;
    case 'M': sendMorsePattern(MORSE_M); break;
    case 'N': sendMorsePattern(MORSE_N); break;
    case 'O': sendMorsePattern(MORSE_O); break;
    case 'P': sendMorsePattern(MORSE_P); break;
    case 'Q': sendMorsePattern(MORSE_Q); break;
    case 'R': sendMorsePattern(MORSE_R); break;
    case 'S': sendMorsePattern(MORSE_S); break;
    case 'T': sendMorsePattern(MORSE_T); break;
    case 'U': sendMorsePattern(MORSE_U); break;
    case 'V': sendMorsePattern(MORSE_V); break;
    case 'W': sendMorsePattern(MORSE_W); break;
    case 'X': sendMorsePattern(MORSE_X); break;
    case 'Y': sendMorsePattern(MORSE_Y); break;
    case 'Z': sendMorsePattern(MORSE_Z); break;
    case '0': sendMorsePattern(MORSE_0); break;
    case '1': sendMorsePattern(MORSE_1); break;
    case '2': sendMorsePattern(MORSE_2); break;
    case '3': sendMorsePattern(MORSE_3); break;
    case '4': sendMorsePattern(MORSE_4); break;
    case '5': sendMorsePattern(MORSE_5); break;
    case '6': sendMorsePattern(MORSE_6); break;
    case '7': sendMorsePattern(MORSE_7); break;
    case '8': sendMorsePattern(MORSE_8); break;
    case '9': sendMorsePattern(MORSE_9); break;
    default: break;
  }
}
