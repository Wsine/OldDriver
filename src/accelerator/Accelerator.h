#ifndef _ACCELERATOR_H
#define _ACCELERATOR_H

#include "uart/rpi_uart.h"

class Accelerator : public rpi_uart {
private:
    bool shiftRelayStatus;

public:
    bool IsShiftRelayON();

    Accelerator();
    ~Accelerator();

    //电压为毫伏 0路为高电压；1路为低电压
    void InputVoltage(int iVoltage);

    //继电器开关 参数：ON/OFF
    bool ShiftRelay(int iOnOrOff);

};

#endif