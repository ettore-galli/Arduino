#ifndef UI_LOGIC_H
#define UI_LOGIC_H
#define DURATIONS_BUFFER_LENGTH 100
class UiLogic {
private:
    bool m_ledStatus;
    int m_startTimeMillis;
    int m_endTimeMillis;
    int* m_durations;
    int m_top;
    int m_timeLastEvent;
    int m_inputTimeoutMillis;

public:
    UiLogic(int inputTimeoutMillis);

    void setStatus(bool ledStatus, int eventTime);
    void reset();
    void add(int duration, int eventTime);
    int *getDurations();
    bool isInputTimeoutPassed(int currentTime);

    bool getStatus() { return m_ledStatus; }
};

#endif