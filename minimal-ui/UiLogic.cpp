#include "UiLogic.h"
#include "Event.h"

UiLogic::UiLogic(int inputTimeoutMillis)
{
    m_top = 0;
    m_timeLastEvent = 0;
    m_inputTimeoutMillis = inputTimeoutMillis;
}

void UiLogic::setStatus(bool ledOn, int eventTime)
{
    m_ledOn = ledOn;
    registerEvent(ledOn, eventTime);
}

void UiLogic::registerEvent(bool ledOn, int eventTime)
{
    if (!m_ledOn) {
        add(eventTime - m_startTimeMillis, eventTime);
    }
    m_startTimeMillis = eventTime;
}

void UiLogic::reset()
{
    for (int i = 0; i < MAX_EVENTS; i++) {
        m_events[i].reset();
    }
    m_top = 0;
}

void UiLogic::add(int duration, int eventTime)
{
    m_timeLastEvent = eventTime;
    m_events[m_top] = Event(true, eventTime, duration);
    m_top++;
}

Event* UiLogic::getEvents() { return m_events; }

int UiLogic::getTop() { return m_top; }

bool UiLogic::isInputTimeoutPassed(int currentTime)
{
    return m_top > 0 && currentTime > m_timeLastEvent + m_inputTimeoutMillis;
}