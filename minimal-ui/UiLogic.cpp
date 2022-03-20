#include "UiLogic.h"
#include "Event.h"

UiLogic::UiLogic(int inputTimeoutMillis)
{
    m_top = 0;
    m_outputLoopOn = false;
    m_inputTimeoutMillis = inputTimeoutMillis;
}

void UiLogic::startLoop() { m_outputLoopOn = true; }

void UiLogic::endLoop() { m_outputLoopOn = false; }

bool UiLogic::isLoopMode() { return m_outputLoopOn; }

void UiLogic::processButtonEvent(bool ledOn, int eventTime)
{
    if (isLoopMode()) {
        m_ledOn = false;
        if (!ledOn) {
            endLoop();
        }
        reset();
        return;
    }

    m_ledOn = ledOn;
    registerEvent(ledOn, eventTime);
}

void UiLogic::registerEvent(bool ledOn, int eventTime) { add(ledOn, eventTime); }

void UiLogic::reset()
{
    for (int i = 0; i < MAX_EVENTS; i++) {
        m_events[i].reset();
    }
    m_top = 0;
}

void UiLogic::add(bool ledOn, int eventTime)
{
    m_events[m_top] = Event(ledOn, eventTime);
    m_top++;
}

Event* UiLogic::getEvents() { return m_events; }

int UiLogic::getTop() { return m_top; }

bool UiLogic::isInputTimeoutPassed(int currentTime)
{
    return (m_top > 0) && (currentTime > m_events[m_top - 1].getEventTime() + m_inputTimeoutMillis);
}

void UiLogic::buildOutputEvents(){
    for (int i = 0; i < getTop(); i++) {
        int duration = (i + 1 < getTop()) ? m_events[i + 1].getEventTime() - m_events[i].getEventTime() : 0;
        m_output_events[i] = OutputEvent(
            m_events[i].isLedOn(),
            m_events[i].getEventTime(),
            duration
        );
    }
}

OutputEvent* UiLogic::getOutputEvents() { return m_output_events; }