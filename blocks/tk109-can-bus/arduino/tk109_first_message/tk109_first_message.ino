// TK109 CAN: one sketch for both ESP32-S3 boards.
// Set SENDER true on one board and false on the other.
//
// Wiring, each ESP32-S3 to its own TK109:
//   TK109 GND -> ESP32 GND     TK109 3V3 -> ESP32 3V3
//   TK109 CTX -> GPIO8         TK109 CRX -> GPIO9
// Bus: CANH to CANH, GND to GND, CANL to CANL.
// Keep both 120R fitted: two boards are the two ends.
//
// Arduino IDE: Tools > Board > esp32 > ESP32S3 Dev Module,
// Tools > USB CDC On Boot > Enabled, then Tools > Port.
// Serial Monitor at 115200. No libraries to install.

#include "driver/twai.h"

const bool SENDER = true;
const int CAN_TX = 8, CAN_RX = 9;
const uint32_t ID = 0x100;

void setup() {
  Serial.begin(115200);
  delay(500);
  twai_general_config_t g = TWAI_GENERAL_CONFIG_DEFAULT(
      (gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_NORMAL);
  twai_timing_config_t t = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  if (twai_driver_install(&g, &t, &f) != ESP_OK ||
      twai_start() != ESP_OK) {
    Serial.println("CAN did not start: check the pins");
    while (true) delay(1000);
  }
  Serial.println(SENDER ? "sender" : "receiver");
}

void loop() {
  if (SENDER) {
    static uint8_t count = 0;
    twai_message_t msg = {};
    msg.identifier = ID;
    msg.data_length_code = 1;
    msg.data[0] = count;
    twai_transmit(&msg, pdMS_TO_TICKS(100));
    delay(1000);
    twai_status_info_t s;
    twai_get_status_info(&s);
    Serial.printf("sent %u, tx errors %lu\n", count++,
                  (unsigned long)s.tx_error_counter);
  } else {
    twai_message_t msg;
    if (twai_receive(&msg, pdMS_TO_TICKS(1000)) == ESP_OK) {
      Serial.printf("got 0x%03lX: %u\n",
                    (unsigned long)msg.identifier, msg.data[0]);
    }
  }
}
