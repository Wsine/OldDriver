#include <stdio.h>
#include "Accelerator.h"
#include "CRC.h"

#define uchar unsigned char
#define ON 1
#define OFF 0

Accelerator::Accelerator() {
    this->shiftRelayStatus = false;
}

Accelerator::~Accelerator() {
    ShiftRelay(OFF); //手动驾驶
    Close();
}

//电压为毫伏 0路为高电压；1路为低电压
void Accelerator::InputVoltage(int iVoltage) {
    uchar uchBuf[13] = { 0x03, 0x10, 0x00, 0x60, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uchar uchVoltage = 0;
    if (iVoltage > 1500) {
        printf("Voltage setting is too large，the value must < 1.5v");
        return;
    }
    uchVoltage = iVoltage >> 8 & 0xFF;
    uchBuf[7] = uchVoltage;
    uchVoltage = iVoltage & 0xFF;
    uchBuf[8] = uchVoltage;

    iVoltage /= 2;
    uchVoltage = iVoltage >> 8 & 0xFF;
    uchBuf[9] = uchVoltage;
    uchVoltage = iVoltage & 0xFF;
    uchBuf[10] = uchVoltage;

    uchar uchResult[2];
    unsigned short ushResult;
    ushResult = CRC16(uchBuf, 11, uchResult);
    uchBuf[11] = uchResult[0];
    uchBuf[12] = uchResult[1];

    Write(uchBuf, 13);
    uchar ucBuffer[256];
    int len;
    len = Read(ucBuffer, 256);

    // if (ucBuffer[0] == 0x03 && ucBuffer[1] == 0x10 && ucBuffer[3] == 0x60) {
    //  printf("Voltage success!\n");
    // }
}

//继电器开关 参数：ON/OFF
bool Accelerator::ShiftRelay(int iOnOrOff) {
    uchar uchResult[2];
    unsigned short ushResult;
    uchar uchBuf[10] = { 0x02, 0x0f, 0x02, 0x00, 0x00, 0x03, 0x01, 0x07, 0x8F, 0x62 };

    if (!iOnOrOff) {
        uchBuf[7] = 0;
    }

    ushResult = CRC16(uchBuf, 8, uchResult);
    uchBuf[8] = uchResult[0];
    uchBuf[9] = uchResult[1];

    Write(uchBuf, 10);
    uchar ucBuffer[256];
    int len;
    len = Read(ucBuffer, 256);

    if (ucBuffer[0] == 0x02 && ucBuffer[1] == 0x0F && ucBuffer[2] == 0x02) {
        // Relay Successfully
        this->shiftRelayStatus = iOnOrOff;
        return true;
    } else {
        return false;
    }
}

bool Accelerator::IsShiftRelayON() {
    return this->shiftRelayStatus;
}
