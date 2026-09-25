# TinkerBlock

Example code for the TinkerBlock modules by Lonely Binary: every block, in
Arduino C++ and MicroPython, and every sketch from the Arduino Beginner
Workshop. This repository holds the code and nothing else.

**The tutorials are at [learn.lonelybinary.com/tinkerblock](https://learn.lonelybinary.com/tinkerblock)**:
what each block is, how it is wired, why the code is written the way it is, and
what to check when it does not work.

The handbook quotes these files at a release tag, so what it explains is
exactly what is here, and a fix made here reaches the handbook at the next tag.

## Layout

```text
blocks/<tk code>-<name>/
  arduino/<sketch>/<sketch>.ino    one folder per sketch, as the Arduino IDE expects
  micropython/<script>.py          ESP32, ESP32-S3 or Raspberry Pi Pico
  extras/                          examples the handbook does not walk through
workshop/<lesson>/<sketch>/<sketch>.ino
```

The handbook sketches open with a comment naming the wiring, counted from the
square pad, and the Arduino IDE settings they need. Open an `.ino` from its own
folder, or copy a `.py` to the board with Thonny.

## Blocks

| Code | Block | Folder | Arduino + MicroPython | Tutorial |
| --- | --- | --- | --- | --- |
| TK01 | XL LED | [`tk01-xl-led`](blocks/tk01-xl-led) | 5 + 5 + extras | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/xl-led) |
| TK02 | RGB LED | [`tk02-rgb-led`](blocks/tk02-rgb-led) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/rgb-led) |
| TK03 | Traffic Light | [`tk03-traffic-light`](blocks/tk03-traffic-light) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/traffic-light) |
| TK04 | Push Button | [`tk04-push-button`](blocks/tk04-push-button) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/push-button) |
| TK05 | Latching Button | [`tk05-latching-button`](blocks/tk05-latching-button) | 4 + 4 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/latching-button) |
| TK06 | Rotary Encoder | [`tk06-rotary-encoder`](blocks/tk06-rotary-encoder) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/rotary-encoder) |
| TK07 | Disc Potentiometer | [`tk07-disc-potentiometer`](blocks/tk07-disc-potentiometer) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/disc-potentiometer) |
| TK08 | Rotary Potentiometer | [`tk08-rotary-potentiometer`](blocks/tk08-rotary-potentiometer) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/rotary-potentiometer) |
| TK09 | Voltmeter | [`tk09-voltmeter`](blocks/tk09-voltmeter) | 4 + 4 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/voltmeter) |
| TK12 | NTC Thermistor | [`tk12-ntc-thermistor`](blocks/tk12-ntc-thermistor) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ntc-thermistor) |
| TK15 | IR Receiver | [`tk15-ir-receiver`](blocks/tk15-ir-receiver) | 5 + 5 + extras | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ir-receiver) |
| TK16 | IR Sender | [`tk16-ir-sender`](blocks/tk16-ir-sender) | 1 + 1 + extras | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ir-sender) |
| TK17 | Collision Sensor | [`tk17-collision-sensor`](blocks/tk17-collision-sensor) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/collision-sensor) |
| TK18 | Hall Effect Sensor | [`tk18-hall-effect-sensor`](blocks/tk18-hall-effect-sensor) | 4 + 4 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/hall-effect-sensor) |
| TK19 | 4-Direction Tilt Sensor | [`tk19-four-direction-tilt-sensor`](blocks/tk19-four-direction-tilt-sensor) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/four-direction-tilt-sensor) |
| TK20 | Ambient Light Sensor | [`tk20-ambient-light-sensor`](blocks/tk20-ambient-light-sensor) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ambient-light-sensor) |
| TK21 | Slim Joystick | [`tk21-slim-joystick`](blocks/tk21-slim-joystick) | 4 + 4 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/slim-joystick) |
| TK23 | Dual Axis Joystick | [`tk23-dual-axis-joystick`](blocks/tk23-dual-axis-joystick) | 4 + 4 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/dual-axis-joystick) |
| TK27 | Analog Microphone | [`tk27-analog-microphone`](blocks/tk27-analog-microphone) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/analog-microphone) |
| TK28 | Knock Sensor | [`tk28-knock-sensor`](blocks/tk28-knock-sensor) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/knock-sensor) |
| TK29 | Analog to Digital Signal | [`tk29-analog-to-digital-signal`](blocks/tk29-analog-to-digital-signal) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/analog-to-digital-signal) |
| TK30 | Vibration Motor | [`tk30-vibration-motor`](blocks/tk30-vibration-motor) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/vibration-motor) |
| TK31 | EEPROM Memory | [`tk31-eeprom-memory`](blocks/tk31-eeprom-memory) | 5 + 5 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/eeprom-memory) |
| TK33 | WS2812 RGB LED Bar | [`tk33-ws2812-led-bar`](blocks/tk33-ws2812-led-bar) | 1 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ws2812-led-bar) |
| TK36 | Active Buzzer | [`tk36-active-buzzer`](blocks/tk36-active-buzzer) | 2 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/active-buzzer) |
| TK37 | Passive Buzzer | [`tk37-passive-buzzer`](blocks/tk37-passive-buzzer) | 3 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/passive-buzzer) |
| TK38 | DHT11 Temperature and Humidity Sensor | [`tk38-dht11`](blocks/tk38-dht11) | 2 + 2 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/dht11) |
| TK39 | DHT22 Temperature and Humidity Sensor | [`tk39-dht22`](blocks/tk39-dht22) | 5 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/dht22) |
| TK41 | Reed Switch | [`tk41-reed-switch`](blocks/tk41-reed-switch) | 2 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/reed-switch) |
| TK42 | LM75 Temperature Sensor | [`tk42-lm75`](blocks/tk42-lm75) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/lm75) |
| TK43 | TTP223 Touch Sensor | [`tk43-tp223-touch-sensor`](blocks/tk43-tp223-touch-sensor) | 2 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/tp223-touch-sensor) |
| TK50 | Ultrasonic Distance Sensor | [`tk50-ultrasonic-sensor`](blocks/tk50-ultrasonic-sensor) | 3 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ultrasonic-sensor) |
| TK51 | TM1637 4-Digit Display (0.56 inch) | [`tk51-tm1637-display`](blocks/tk51-tm1637-display) | 4 + 4 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/tm1637-display) |
| TK52 | 8x8 LED Matrix | [`tk52-matrix-led`](blocks/tk52-matrix-led) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/matrix-led) |
| TK53 | 74HC595 Segment LED | [`tk53-hc595-segment-led`](blocks/tk53-hc595-segment-led) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/hc595-segment-led) |
| TK57 | Reflective Optical Sensor | [`tk57-reflective-optical-sensor`](blocks/tk57-reflective-optical-sensor) | 2 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/reflective-optical-sensor) |
| TK59 | Piezo-Ceramic Sensor | [`tk59-piezo-sensor`](blocks/tk59-piezo-sensor) | 2 + 2 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/piezo-sensor) |
| TK61 | Infrared Speed Sensor | [`tk61-ir-speed-sensor`](blocks/tk61-ir-speed-sensor) | 2 + 2 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ir-speed-sensor) |
| TK62 | Tilt Sensor | [`tk62-tilt-sensor`](blocks/tk62-tilt-sensor) | 2 + 2 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/tilt-sensor) |
| TK63 | Infrared Transmitter | [`tk63-ir-transmitter`](blocks/tk63-ir-transmitter) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ir-transmitter) |
| TK64 | Infrared Photodiode | [`tk64-ir-photodiode`](blocks/tk64-ir-photodiode) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ir-photodiode) |
| TK65 | Steam Sensor | [`tk65-steam-sensor`](blocks/tk65-steam-sensor) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/steam-sensor) |
| TK70 | Linear Hall Effect Sensor | [`tk70-linear-hall-sensor`](blocks/tk70-linear-hall-sensor) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/linear-hall-sensor) |
| TK85 | Matrix Keypad with I2C Adapter (4x4, PCF8574) | [`tk85-matrix-keypad`](blocks/tk85-matrix-keypad) | 4 + 1 + extras | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/matrix-keypad) |
| TK89 | 1.8-inch TFT Display | [`tk89-tft-1-8-inch`](blocks/tk89-tft-1-8-inch) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/tft-1-8-inch) |
| TK94 | Five-Direction Joystick | [`tk94-five-direction-joystick`](blocks/tk94-five-direction-joystick) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/five-direction-joystick) |
| TK95 | Dual Bright LEDs | [`tk95-dual-bright-leds`](blocks/tk95-dual-bright-leds) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/dual-bright-leds) |
| TK96 | Mechanical Key and LED | [`tk96-mechanical-key`](blocks/tk96-mechanical-key) | 1 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/mechanical-key) |
| TK100 | I2S Amplifier | [`tk100-i2s-amplifier`](blocks/tk100-i2s-amplifier) | 2 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/i2s-amplifier) |
| TK101 | PDM Microphone | [`tk101-pdm-microphone`](blocks/tk101-pdm-microphone) | 2 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/pdm-microphone) |
| TK103 | I2S DAC | [`tk103-i2s-dac`](blocks/tk103-i2s-dac) | 2 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/i2s-dac) |
| TK109 | CAN Bus | [`tk109-can-bus`](blocks/tk109-can-bus) | 1 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/can-bus) |
| TK119 | INA219 Current Monitor | [`tk119-ina219`](blocks/tk119-ina219) | 3 + 0 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/ina219) |
| TK120 | SHT31 Temperature and Humidity Sensor | [`tk120-sht31`](blocks/tk120-sht31) | 3 + 1 | [handbook](https://learn.lonelybinary.com/tinkerblock/blocks/sht31) |

TK97 (logic level converter) has no code: it is wiring only.

## Arduino Beginner Workshop

| Lesson | Title | Folder |
| --- | --- | --- |
| 02 | [2. First Program: Blink](https://learn.lonelybinary.com/guides/tinkerblock-workshop/02-first-program-blink) | [`02-first-program-blink`](workshop/02-first-program-blink) |
| 03 | [3. Digital Input: Button](https://learn.lonelybinary.com/guides/tinkerblock-workshop/03-digital-input-button) | [`03-digital-input-button`](workshop/03-digital-input-button) |
| 04 | [4. Traffic Light](https://learn.lonelybinary.com/guides/tinkerblock-workshop/04-traffic-light) | [`04-traffic-light`](workshop/04-traffic-light) |
| 05 | [5. Serial Monitor Debugging](https://learn.lonelybinary.com/guides/tinkerblock-workshop/05-serial-monitor-debugging) | [`05-serial-monitor-debugging`](workshop/05-serial-monitor-debugging) |
| 06 | [6. RGB LED](https://learn.lonelybinary.com/guides/tinkerblock-workshop/06-rgb-led) | [`06-rgb-led`](workshop/06-rgb-led) |
| 07 | [7. Latching Button](https://learn.lonelybinary.com/guides/tinkerblock-workshop/07-latching-button) | [`07-latching-button`](workshop/07-latching-button) |
| 08 | [8. Collision Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/08-collision-sensor) | [`08-collision-sensor`](workshop/08-collision-sensor) |
| 09 | [9. Touch Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/09-touch-sensor) | [`09-touch-sensor`](workshop/09-touch-sensor) |
| 10 | [10. Tilt Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/10-tilt-sensor) | [`10-tilt-sensor`](workshop/10-tilt-sensor) |
| 11 | [11. Potentiometers](https://learn.lonelybinary.com/guides/tinkerblock-workshop/11-potentiometers) | [`11-potentiometers`](workshop/11-potentiometers) |
| 12 | [12. NTC Thermistor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/12-ntc-thermistor) | [`12-ntc-thermistor`](workshop/12-ntc-thermistor) |
| 13 | [13. Ambient Light Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/13-ambient-light-sensor) | [`13-ambient-light-sensor`](workshop/13-ambient-light-sensor) |
| 14 | [14. Knock Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/14-knock-sensor) | [`14-knock-sensor`](workshop/14-knock-sensor) |
| 15 | [15. Multiple Sensors with Serial](https://learn.lonelybinary.com/guides/tinkerblock-workshop/15-multiple-sensors-with-serial) | [`15-multiple-sensors-with-serial`](workshop/15-multiple-sensors-with-serial) |
| 16 | [16. PWM Fading LED](https://learn.lonelybinary.com/guides/tinkerblock-workshop/16-pwm-fading-led) | [`16-pwm-fading-led`](workshop/16-pwm-fading-led) |
| 17 | [17. Passive Buzzer](https://learn.lonelybinary.com/guides/tinkerblock-workshop/17-passive-buzzer) | [`17-passive-buzzer`](workshop/17-passive-buzzer) |
| 18 | [18. Active Buzzer](https://learn.lonelybinary.com/guides/tinkerblock-workshop/18-active-buzzer) | [`18-active-buzzer`](workshop/18-active-buzzer) |
| 19 | [19. Vibration Motor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/19-vibration-motor) | [`19-vibration-motor`](workshop/19-vibration-motor) |
| 20 | [20. RGB Color Mixing](https://learn.lonelybinary.com/guides/tinkerblock-workshop/20-rgb-color-mixing) | [`20-rgb-color-mixing`](workshop/20-rgb-color-mixing) |
| 21 | [21. TM1637 4-Digit Display](https://learn.lonelybinary.com/guides/tinkerblock-workshop/21-tm1637-4-digit-display) | [`21-tm1637-4-digit-display`](workshop/21-tm1637-4-digit-display) |
| 22 | [22. Matrix LED](https://learn.lonelybinary.com/guides/tinkerblock-workshop/22-matrix-led) | [`22-matrix-led`](workshop/22-matrix-led) |
| 23 | [23. Custom Functions](https://learn.lonelybinary.com/guides/tinkerblock-workshop/23-custom-functions) | [`23-custom-functions`](workshop/23-custom-functions) |
| 24 | [24. 1.8 Inch TFT Display](https://learn.lonelybinary.com/guides/tinkerblock-workshop/24-1-8-inch-tft-display) | [`24-1-8-inch-tft-display`](workshop/24-1-8-inch-tft-display) |
| 25 | [25. WS2812 RGB LED Bar](https://learn.lonelybinary.com/guides/tinkerblock-workshop/25-ws2812-rgb-led-bar) | [`25-ws2812-rgb-led-bar`](workshop/25-ws2812-rgb-led-bar) |
| 26 | [26. IR Communication](https://learn.lonelybinary.com/guides/tinkerblock-workshop/26-ir-communication) | [`26-ir-communication`](workshop/26-ir-communication) |
| 27 | [27. NTC Thermistor Temperature Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/27-ntc-thermistor-temperature-sensor) | [`27-ntc-thermistor-temperature-sensor`](workshop/27-ntc-thermistor-temperature-sensor) |
| 28 | [28. EEPROM Memory](https://learn.lonelybinary.com/guides/tinkerblock-workshop/28-eeprom-memory) | [`28-eeprom-memory`](workshop/28-eeprom-memory) |
| 29 | [29. ADC Concept](https://learn.lonelybinary.com/guides/tinkerblock-workshop/29-adc-concept) | [`29-adc-concept`](workshop/29-adc-concept) |
| 30 | [30. Rotary Encoder](https://learn.lonelybinary.com/guides/tinkerblock-workshop/30-rotary-encoder) | [`30-rotary-encoder`](workshop/30-rotary-encoder) |
| 31 | [31. I2C Temperature LM75](https://learn.lonelybinary.com/guides/tinkerblock-workshop/31-i2c-temperature-lm75) | [`31-i2c-temperature-lm75`](workshop/31-i2c-temperature-lm75) |
| 32 | [32. I2C Matrix Keypad](https://learn.lonelybinary.com/guides/tinkerblock-workshop/32-i2c-matrix-keypad) | [`32-i2c-matrix-keypad`](workshop/32-i2c-matrix-keypad) |
| 33 | [33. Steam Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/33-steam-sensor) | [`33-steam-sensor`](workshop/33-steam-sensor) |
| 34 | [34. Joystick](https://learn.lonelybinary.com/guides/tinkerblock-workshop/34-joystick) | [`34-joystick`](workshop/34-joystick) |
| 35 | [35. Voltmeter](https://learn.lonelybinary.com/guides/tinkerblock-workshop/35-voltmeter) | [`35-voltmeter`](workshop/35-voltmeter) |
| 36 | [36. Piezo Sensor](https://learn.lonelybinary.com/guides/tinkerblock-workshop/36-piezo-sensor) | [`36-piezo-sensor`](workshop/36-piezo-sensor) |
| 37 | [37. Extreme Bright Dual LEDs](https://learn.lonelybinary.com/guides/tinkerblock-workshop/37-extreme-bright-dual-leds) | [`37-extreme-bright-dual-leds`](workshop/37-extreme-bright-dual-leds) |
| 38 | [38. Project: Temperature Humidity Display](https://learn.lonelybinary.com/guides/tinkerblock-workshop/38-project-temperature-humidity-display) | [`38-project-temperature-humidity-display`](workshop/38-project-temperature-humidity-display) |
| 39 | [39. Project: Button Buzzer LED](https://learn.lonelybinary.com/guides/tinkerblock-workshop/39-project-button-buzzer-led) | [`39-project-button-buzzer-led`](workshop/39-project-button-buzzer-led) |
| 40 | [40. Project: Potentiometer Control](https://learn.lonelybinary.com/guides/tinkerblock-workshop/40-project-potentiometer-control) | [`40-project-potentiometer-control`](workshop/40-project-potentiometer-control) |
| 41 | [41. Project: IR Remote LED](https://learn.lonelybinary.com/guides/tinkerblock-workshop/41-project-ir-remote-led) | [`41-project-ir-remote-led`](workshop/41-project-ir-remote-led) |
| 42 | [42. Project: Joystick Control](https://learn.lonelybinary.com/guides/tinkerblock-workshop/42-project-joystick-control) | [`42-project-joystick-control`](workshop/42-project-joystick-control) |
| 43 | [43. Project: EEPROM Settings](https://learn.lonelybinary.com/guides/tinkerblock-workshop/43-project-eeprom-settings) | [`43-project-eeprom-settings`](workshop/43-project-eeprom-settings) |
| 44 | [44. Project: TFT Sensor Display](https://learn.lonelybinary.com/guides/tinkerblock-workshop/44-project-tft-sensor-display) | [`44-project-tft-sensor-display`](workshop/44-project-tft-sensor-display) |
| 45 | [45. Final Project: Free Integration](https://learn.lonelybinary.com/guides/tinkerblock-workshop/45-final-project-free-integration) | [`45-final-project-free-integration`](workshop/45-final-project-free-integration) |

## Questions and bugs

- **A bug in the code:** open an issue here with the block, the development
  board, the sketch and its exact output.
- **A question about using a block:** ask in the discussion at the foot of that
  block's page on [learn.lonelybinary.com](https://learn.lonelybinary.com/tinkerblock/blocks).
