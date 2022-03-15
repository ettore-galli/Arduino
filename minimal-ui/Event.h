#ifndef EVENT_H
#define EVENT_H

class Event {

private:
    bool m_ledOn;
    int m_eventTime;

public:
    Event() {};
    Event(bool ledOn, int eventTime, int duration)
    {
        m_ledOn = ledOn;
        m_eventTime = eventTime;
    };
    void reset()
    {
        m_ledOn = false;
        m_eventTime = 0;
    }
    bool isLedOn() { return m_ledOn; }
    int getEventTime() { return m_eventTime; }
};

#endif