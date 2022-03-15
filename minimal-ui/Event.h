#ifndef EVENT_H
#define EVENT_H

class Event {

private:
    bool m_ledOn;
    int m_eventTime;
    int m_duration;

public:
    Event() {};
    Event(bool ledOn, int eventTime, int duration)
    {
        m_ledOn = ledOn;
        m_eventTime = eventTime;
        m_duration = duration;
    };
    void reset()
    {
        m_ledOn = false;
        m_eventTime = 0;
        m_duration = 0;
    }
    bool isLedOn() { return m_ledOn; }
    int getEventTime() { return m_eventTime; }
    int getDuration() { return m_duration; }
};

#endif