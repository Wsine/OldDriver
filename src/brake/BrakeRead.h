#ifndef _BRAKE_READ_H
#define _BRAKE_READ_H

#include "uart/rpi_uart.h"

class BrakeRead : public rpi_uart{
private:
    int iIsHead;
    int iBufferLen;
    int iBufferFlag;
    unsigned char ucBuffer[5];
    unsigned short ushEncoderValue;
public:
    BrakeRead();
    ~BrakeRead();
    void OnReceive();
    unsigned short getValue();
};

#endif
