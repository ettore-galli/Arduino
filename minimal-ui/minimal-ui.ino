#include "UiLogic.h"

/* MINIMAL UI */

#define BAUD_RATE 115200

#define INPUT_TIMEOUT_MILLIS 1000

const int BUTTON_INPUT = 2;
const int LED_OUTPUT = 7;

volatile boolean ledOn = false;

UiLogic logic = UiLogic(INPUT_TIMEOUT_MILLIS);

void setupSerial() { Serial.begin(BAUD_RATE); }

void setupGPIO()
{
    pinMode(LED_OUTPUT, OUTPUT);
    pinMode(BUTTON_INPUT, INPUT_PULLUP);
}

void setupButtonInterrupt() { attachInterrupt(digitalPinToInterrupt(BUTTON_INPUT), change, CHANGE); }

void change() { logic.setStatus(!digitalRead(BUTTON_INPUT), millis()); }

void RefreshOut()
{
    delay(10);
    digitalWrite(LED_OUTPUT, logic.getStatus());
    if (logic.isInputTimeoutPassed(millis())){
        for (int i=0; i<3; i++){
            digitalWrite(LED_OUTPUT, HIGH);
            delay(50);
            digitalWrite(LED_OUTPUT, LOW);
            delay(50);
        }
        logic.reset();
    }
}

void setup()
{
    setupSerial();
    setupGPIO();
    setupButtonInterrupt();
}

void loop() { RefreshOut(); }
