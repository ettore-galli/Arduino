#include "Event.h"

#ifndef UI_LOGIC_H
#define UI_LOGIC_H
#define MAX_EVENTS 100

using namespace std;

class UiLogic {
private:
    bool m_ledOn;

    int m_startTimeMillis;
    int m_endTimeMillis;
    Event m_events[MAX_EVENTS];
    int m_top;
    int m_timeLastEvent;
    int m_inputTimeoutMillis;

public:
    UiLogic(int inputTimeoutMillis);

    bool isLedOn() { return m_ledOn; }
    void setStatus(bool ledOn, int eventTime);

    void registerEvent(bool ledOn, int eventTime);
    void reset();
    void add(int duration, int eventTime);
    Event* getEvents();
    int getTop();
    bool isInputTimeoutPassed(int currentTime);
};

#endif