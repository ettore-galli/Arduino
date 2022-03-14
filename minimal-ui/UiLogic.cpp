#include "UiLogic.h"

UiLogic::UiLogic(int inputTimeoutMillis)
{
    m_top = 0;
    m_durations = new int[DURATIONS_BUFFER_LENGTH];
    m_timeLastEvent = 0;
    m_inputTimeoutMillis = inputTimeoutMillis;
}

void UiLogic::setStatus(bool ledStatus, int eventTime)
{
    m_ledStatus = ledStatus;

    if (!m_ledStatus) {
        add(eventTime - m_startTimeMillis, eventTime);
    }
    m_startTimeMillis = eventTime;
}

void UiLogic::reset()
{
    for (int i = 0; i < DURATIONS_BUFFER_LENGTH; i++) {
        m_durations[i] = 0;
    }
    m_top = 0;
}

void UiLogic::add(int duration, int eventTime)
{
    m_timeLastEvent = eventTime;
    m_durations[m_top++] = duration;
}

int* UiLogic::getDurations() { return m_durations; }

int UiLogic::getTop() { return m_top; }

bool UiLogic::isInputTimeoutPassed(int currentTime)
{
    return m_top > 0 && currentTime > m_timeLastEvent + m_inputTimeoutMillis;
}