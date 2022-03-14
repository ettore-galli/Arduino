#include "UiLogic.h"

UiLogic::UiLogic(int inputTimeoutMillis)
{
    m_top = 0;
    m_timeLastEvent = 0;
    m_inputTimeoutMillis = inputTimeoutMillis;
}

void UiLogic::setStatus(bool ledStatus, int eventTime)
{
    m_ledStatus = ledStatus;
    if (m_ledStatus) {
        m_startTimeMillis = eventTime;
    } else {
        add(eventTime - m_startTimeMillis, eventTime);
    }
}

void UiLogic::reset()
{
    for (int i = 0; i < m_top; i++) {
        m_durations[i] = 0;
    }
    m_top = 0;
}

void UiLogic::add(int duration, int eventTime)
{
    m_durations[m_top++] = duration;
    m_timeLastEvent = eventTime;
}

int *UiLogic::getDurations()
{
    return m_durations;
}

bool UiLogic::isInputTimeoutPassed(int currentTime)
{
    return m_top > 0 && currentTime > m_timeLastEvent + m_inputTimeoutMillis;
}