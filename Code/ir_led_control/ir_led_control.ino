/*
 * Infrared Remote-Controlled LED System
 * -------------------------------------
 * Decodes NEC IR remote commands and lights the LED mapped to each button.
 *
 * Controller used in the reconstructed implementation: Arduino Uno.
 * Library: IRremote (v4.x) from the Arduino Library Manager.
 *
 * Wiring:
 *   IR receiver OUT -> D2
 *   LEDs (via 220R) -> D8, D9, D10, D11
 */

#include <IRremote.hpp>

const int IR_RECEIVER_PIN = 2;

const int ledPins[]  = {8, 9, 10, 11};   // red, green, blue, yellow
const int ledCount   = sizeof(ledPins) / sizeof(ledPins[0]);

// Replace these with the command values printed for YOUR remote.
const uint8_t buttonCommands[] = {
  0x45,   // button 1 -> red
  0x46,   // button 2 -> green
  0x47,   // button 3 -> blue
  0x44    // button 4 -> yellow
};

void allOff() {
  for (int j = 0; j < ledCount; j++) {
    digitalWrite(ledPins[j], LOW);
  }
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);
  Serial.println(F("IR LED Control System Ready"));
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t command = IrReceiver.decodedIRData.command;

    Serial.print(F("Received command: 0x"));
    Serial.println(command, HEX);

    for (int i = 0; i < ledCount; i++) {
      if (command == buttonCommands[i]) {
        allOff();                         // mutually exclusive outputs
        digitalWrite(ledPins[i], HIGH);
        break;
      }
    }

    IrReceiver.resume();                  // re-arm the receiver for the next frame
  }
}
