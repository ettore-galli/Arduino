#include <ArduinoUnit.h>

#include "Event.h"
#include "UiLogic.h"

/* MINIMAL UI */

#define BAUD_RATE 115200

#define INPUT_TIMEOUT_MILLIS 1000
#define INTER_LOOP_DELAY_MILLIS 1000

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

bool readWithDebouce(int inputPin)
{
    int N = 5;
    int d = 1;
    int r = 0;
    for (int i = 0; i < N; i++) {
        if (!digitalRead(inputPin)) {
            r++;
        }
        delay(d);
    }
    return r > N - 1;
}

void change() { logic.processButtonEvent(readWithDebouce(BUTTON_INPUT), millis()); }

void outputLedStatus()
{
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_OUTPUT, logic.isLedOn());
}

void checkAndStartLoopMode()
{
    if (logic.isInputTimeoutPassed(millis())) {
        logic.startLoop();
    }
}

void displayEventsSequence(Event* events)
{
    for (int i = 0; i < logic.getTop(); i++) {

        if (!logic.isLoopMode()) {
            break;
        }

        digitalWrite(LED_OUTPUT, events[i].isLedOn());

        // TODO: Move this calculation into logic class
        int duration = (i + 1 < logic.getTop()) ? events[i + 1].getEventTime() - events[i].getEventTime() : 0;

        delay(duration);
    }
}

void echoLoop()
{
    if (!logic.isLoopMode()) {
        return;
    }

    digitalWrite(LED_BUILTIN, HIGH);

    Event* events = logic.getEvents();
    displayEventsSequenceInLoop(events);
    displayEventsEndSignal();
}

void displayEventsSequenceInLoop(Event* events)
{
    while (logic.isLoopMode()) {
        delay(INTER_LOOP_DELAY_MILLIS);
        displayEventsSequence(events);
    }
}

void displayEventsEndSignal()
{
    int interval = 50;
    digitalWrite(LED_OUTPUT, LOW);
    delay(3 + interval);
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_OUTPUT, HIGH);
        delay(interval);
        digitalWrite(LED_OUTPUT, LOW);
        delay(interval);
    }
}

void DisplayOutput()
{
    outputLedStatus();
    checkAndStartLoopMode();
    echoLoop();
}

void setup()
{
    setupSerial();
    setupGPIO();
    setupButtonInterrupt();
    logic.reset();
}

void loop() { DisplayOutput(); }
