#ifndef UI_LOGIC_H
#define UI_LOGIC_H

class UiLogic {
private:
    bool m_ledStatus;

public:
    UiLogic();

    void setStatus(bool ledStatus);

    bool getStatus() { return m_ledStatus; }
};

#endif