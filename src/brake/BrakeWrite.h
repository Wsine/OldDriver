#ifndef _BRAKE_WRITE_H
#define _BRAKE_WRITE_H

#include "uart/rpi_uart.h"

class BrakeWrite : public rpi_uart {
private:
    unsigned short aim;
    unsigned short current;
    bool isTurning;
    void TurnDirection(int leftOrRight, int speed);
    void Stop();
public:
    BrakeWrite();
    ~BrakeWrite();
    void setAim(unsigned short _aim);
    void setCurrent(unsigned short _current);
    bool TurnToAim();
    bool Reset();
};

#endif
