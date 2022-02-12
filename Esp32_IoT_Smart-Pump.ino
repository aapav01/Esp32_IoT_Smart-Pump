
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

#define CLOSE 1
#define OPEN 0

/*
   Previous State
*/
bool prev_state_switch_1 = CLOSE;
bool pump_was_on = false;

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

  bool switchState_1 = digitalRead(SWITCH_INPUT_1);
  bool tankFull = digitalRead(SWITCH_INPUT_2);

  // Toggle The Pump if its on and off if its on.
  if (switchState_1 == LOW && tankFull == HIGH) {
    Serial.print("Switch 1 : ");
    prev_state_switch_1 = !prev_state_switch_1;
    pump_was_on = !pump_was_on;
    Serial.println(!prev_state_switch_1 ? "ON" : "OFF");
    digitalWrite(RELAY_OUTPUT_1, prev_state_switch_1);
    Blynk.virtualWrite(V1, !prev_state_switch_1);
    delay(1000);
  }
  // Check Tank is Full and Update the Cloud about the State and Turn off the Pump
  if (tankFull == LOW && BlynkState::get() != MODE_TANK_FULL) {
    Serial.println("Tank Full");
    prev_state_switch_1 = CLOSE;
    pump_was_on = true;
    digitalWrite(RELAY_OUTPUT_1, prev_state_switch_1);
    Blynk.virtualWrite(V1, !prev_state_switch_1); \
    Blynk.virtualWrite(V0, true);
    delay(1000);
  }
  // When Tank was full but its not anymore And also pump was running before
  // We will reset the virtual pin to false to tell the tank is not full.
  // And Device is ready to accept command
  // TODO: Check Cloud Connection
  if (tankFull == HIGH && pump_was_on == true) {
    Blynk.virtualWrite(V0, false);
    pump_was_on = false;
  }
}

/*
   Control relay using Blynk Virtual Pins
   This special function is called BLYNK_WRITE. Think of it
   as meaning that the Blynk.Cloud is telling your hardware
   “there a new value written to your virtual pin”.
*/
BLYNK_WRITE(V1) {
  digitalWrite(RELAY_OUTPUT_1, !param.asInt());
  prev_state_switch_1 = !param.asInt();

#ifdef APP_DEBUG
  Serial.print("Virtual Pin 1 Changed to: ");
  Serial.println(param.asInt() ? "ON" : "OFF");
#endif
}

/*
   Request the latest state from Cloud
   Requests all latest stored values for all widgets. All
   analog/digital/virtual pin values and states will be
   set to the latest stored value. Every virtual pin will
   generate BLYNK_WRITE() event.
*/
BLYNK_CONNECTED() {
  Blynk.syncAll();
}
