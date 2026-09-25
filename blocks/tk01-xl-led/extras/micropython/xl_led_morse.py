"""
TK01-XL LED - Morse Code Demo (Raspberry Pi Pico 2)

Description:
Uses LED on/off to display Morse code. Edit MESSAGE below; the LED will flash
that string in a loop with a 2-second pause between repeats. Supports A-Z and 0-9. Space = pause between words.

Wiring:
Change LED_PIN below to match your wiring.

Usage:
1. Set MESSAGE to your text (e.g. "TK01" or "HELLO")
2. Save and run on Raspberry Pi Pico 2
3. Terminal prints the character being sent
"""

from machine import Pin
import time

# Connect to TK01 SIGNAL; change to match your wiring
LED_PIN = 0

# ----- Edit this string (A-Z, 0-9, space). LED will send it in Morse in a loop. -----
MESSAGE = "SOS"

# Time units (ms)
DOT_MS = 200
DASH_MS = DOT_MS * 3
ELEMENT_GAP_MS = DOT_MS
LETTER_GAP_MS = DOT_MS * 3
WORD_GAP_MS = DOT_MS * 7

MORSE = {
    'A': '.-',    'B': '-...',  'C': '-.-.',  'D': '-..',   'E': '.',     'F': '..-.',
    'G': '--.',   'H': '....',  'I': '..',    'J': '.---',  'K': '-.-',   'L': '.-..',
    'M': '--',    'N': '-.',    'O': '---',   'P': '.--.',  'Q': '--.-',  'R': '.-.',
    'S': '...',   'T': '-',     'U': '..-',   'V': '...-',  'W': '.--',   'X': '-..-',
    'Y': '-.--',  'Z': '--..',
    '0': '-----', '1': '.----', '2': '..---', '3': '...--', '4': '....-', '5': '.....',
    '6': '-....', '7': '--...', '8': '---..', '9': '----.',
}

def ms(n):
    return n / 1000.0

led = Pin(LED_PIN, Pin.OUT)

def led_on(ms_val):
    led.on()
    time.sleep(ms(ms_val))

def led_off(ms_val):
    led.off()
    time.sleep(ms(ms_val))

def send_dot():
    led_on(DOT_MS)
    led_off(ELEMENT_GAP_MS)

def send_dash():
    led_on(DASH_MS)
    led_off(ELEMENT_GAP_MS)

def send_morse_char(c):
    c = c.upper()
    if c not in MORSE:
        return
    print("Sending:", c)
    for sym in MORSE[c]:
        if sym == '.':
            send_dot()
        else:
            send_dash()

def main():
    print("Morse - loop, 2s pause between repeats")
    while True:
        for c in MESSAGE:
            if c == ' ':
                time.sleep(ms(WORD_GAP_MS))
                print(" [space]")
            else:
                send_morse_char(c)
                time.sleep(ms(LETTER_GAP_MS))
        print("Done")
        time.sleep(2)  # Pause 2 seconds before next repeat

if __name__ == "__main__":
    main()
