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

bool readWithDebouce(int inputPin){
    int N=5;
    int d=1;
    int r=0;
    for (int i=0; i<N; i++){
        if (!digitalRead(inputPin)){
            r++;
        }
        delay(d);
    }
    return r > N - 1;
}

void change() { logic.setStatus(readWithDebouce(BUTTON_INPUT), millis()); }

void RefreshOut()
{
    delay(10);
    digitalWrite(LED_OUTPUT, logic.getStatus());
    if (logic.isInputTimeoutPassed(millis())){
        int* durations = logic.getDurations();
        Serial.println("-----");
        for (int i=0; i<logic.getTop(); i++){
            Serial.println(durations[i]);
        }
        for (int i=0; i<logic.getTop(); i++){
            digitalWrite(LED_OUTPUT, HIGH);
            delay(durations[i]);
            digitalWrite(LED_OUTPUT, LOW);
            delay(durations[i]);
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