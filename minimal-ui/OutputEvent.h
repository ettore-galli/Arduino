#ifndef OUTPUT_EVENT_H
#define OUTPUT_EVENT_H

class OutputEvent {

private:
    bool m_ledOn;
    int m_eventTime;
    int m_duration;

public:
    OutputEvent() {};
    OutputEvent(bool ledOn, int eventTime, int duration)
    {
        m_ledOn = ledOn;
        m_eventTime = eventTime;
        m_duration = duration;

    };
    bool isLedOn() { return m_ledOn; }
    int getEventTime() { return m_eventTime; }
    int getDuration() { return m_duration; }
};

#endif