#include "Event.h"
#include "OutputEvent.h"

#ifndef UI_LOGIC_H
#define UI_LOGIC_H
#define MAX_EVENTS 100

using namespace std;

class UiLogic {
private:
    bool m_ledOn;

    Event m_events[MAX_EVENTS];
    OutputEvent m_output_events[MAX_EVENTS];

    int m_top;
    int m_inputTimeoutMillis;

public:
    UiLogic(int inputTimeoutMillis);

    bool isLedOn() { return m_ledOn; }

    void processButtonEvent(bool ledOn, int eventTime);

    void registerEvent(bool ledOn, int eventTime);
    void reset();

    void add(bool ledOn, int eventTime);
    Event* getEvents();
    int getTop();
    bool isInputTimeoutPassed(int currentTime);

    void buildOutputEvents();
    OutputEvent* getOutputEvents();
};

#endif