#include <ArduinoUnit.h>

#include "Event.h"
#include "OutputEvent.h"
#include "UiLogic.h"

/* MINIMAL UI */

#define BAUD_RATE 115200

#define INPUT_TIMEOUT_MILLIS 1000
#define INTER_LOOP_DELAY_MILLIS 1000
#define EVENTS_END_SIGNAL_DELAY 50

enum class debouncedReadings : char { high = 1, low = -1, undecidable = 0 };
const int BUTTON_INPUT = 2;
const int LED_OUTPUT = 7;

UiLogic logic = UiLogic(INPUT_TIMEOUT_MILLIS);

void setupSerial() { Serial.begin(BAUD_RATE); }

void setupGPIO()
{
    pinMode(LED_OUTPUT, OUTPUT);
    pinMode(BUTTON_INPUT, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
}

void setupButtonInterrupt() { attachInterrupt(digitalPinToInterrupt(BUTTON_INPUT), change, CHANGE); }

debouncedReadings readWithDebouce(int inputPin)
{
    int N = 3;
    int d = 5;
    int r = 0;
    for (int i = 0; i < 3 * N; i++) {
        if (!digitalRead(inputPin)) {
            r++;
        }
        delay(d);
    }
    if (r > N - 2) {
        return debouncedReadings ::high;
    }
    if (r < 2) {
        return debouncedReadings ::low;
    }
    return debouncedReadings ::undecidable;
}

void change()
{
    debouncedReadings reading = readWithDebouce(BUTTON_INPUT);
    if (reading != debouncedReadings ::undecidable) {
        Serial.println(reading == debouncedReadings ::high ? "ON" : "OFF");
        logic.processButtonEvent(reading == debouncedReadings ::high, millis());
    }
}

void outputLedStatus()
{
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_OUTPUT, logic.isLedOn());
}

void displayEventsSequence(OutputEvent* events)
{
    for (int i = 0; i < logic.getTop(); i++) {

        digitalWrite(LED_OUTPUT, events[i].isLedOn());
        delay(events[i].getDuration());
    }
}

void DisplayOutput()
{
    outputLedStatus();

    if (logic.isInputTimeoutPassed(millis())) {
        logic.buildOutputEvents();
        OutputEvent* events = logic.getOutputEvents();
        displayEventsSequence(events);
        logic.reset();
    }
}

void setup()
{
    setupSerial();
    setupGPIO();
    setupButtonInterrupt();
    logic.reset();
}

void loop() { DisplayOutput(); }
