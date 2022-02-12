
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLl6ycCMT5"
#define BLYNK_DEVICE_NAME "IoT Pump"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7

#include "BlynkEdgent.h"

#define SWITCH_INPUT_1 27
#define SWITCH_INPUT_2 4
#define RELAY_OUTPUT_1 26

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();

  pinMode(SWITCH_INPUT_1, INPUT_PULLUP);
  pinMode(SWITCH_INPUT_2, INPUT_PULLUP);
  pinMode(RELAY_OUTPUT_1, OUTPUT);
  digitalWrite(RELAY_OUTPUT_1, HIGH);
  Blynk.virtualWrite(V0, false);
}

void loop() {
  BlynkEdgent.run();
}
