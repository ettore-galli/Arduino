#include "UiLogic.h"
#include "Event.h"

UiLogic::UiLogic(int inputTimeoutMillis)
{
    m_top = 0;
    m_outputLoopOn = false;
    m_inputTimeoutMillis = inputTimeoutMillis;
}

void UiLogic::startLoop() { m_outputLoopOn = true; }

void UiLogic::endLoop()
{
    m_outputLoopOn = false;
}

bool UiLogic::isLoopMode() { return m_outputLoopOn; }

void UiLogic::processButtonEvent(bool ledOn, int eventTime)
{
    if (isLoopMode()){
        m_ledOn = false;
        if (!ledOn){
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
    m_events[m_top] = Event(ledOn, eventTime, 0);
    m_top++;
}

Event* UiLogic::getEvents() { return m_events; }

int UiLogic::getTop() { return m_top; }

bool UiLogic::isInputTimeoutPassed(int currentTime)
{
    return (m_top > 0) && (currentTime > m_events[m_top - 1].getEventTime() + m_inputTimeoutMillis);
}
