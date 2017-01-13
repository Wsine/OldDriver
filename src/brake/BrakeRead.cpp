#include "BrakeRead.h"

BrakeRead::BrakeRead() {
    this->iIsHead = 0;
    this->iBufferLen = 0;
    this->iBufferFlag = 0;
    this->ushEncoderValue = 0;
    memset(this->ucBuffer, 0, 5);
}

BrakeRead::~BrakeRead() {
    Close();
}

void BrakeRead::OnReceive() {
    unsigned char buffer[10];
    int dwRealRead = Read(buffer, 10);
    for (int i = 0; i < dwRealRead; ++i) {
        switch (iIsHead) {
        case 0:
            if (buffer[i] == 0xFF) {
                iIsHead = 1;
                ucBuffer[iBufferLen++] = buffer[i];
            }
            break;
        case 1:
            if (buffer[i] == 0x81) {
                iIsHead = 2;
                ucBuffer[iBufferLen++] = buffer[i];
            }
            break;
        case 2:
            if (iBufferLen < 5) {
                ucBuffer[iBufferLen++] = buffer[i];
                if (iBufferLen > 4) {
                    iIsHead = 0;
                    iBufferLen = 0;
                    iBufferFlag = 1;
                    ushEncoderValue = 0;
                }
            }
            break;
        }
        if (iBufferFlag == 1) {
            if (ucBuffer[2] + ucBuffer[3] == ucBuffer[4]) {
                unsigned short ushTemp;
                ushEncoderValue = ucBuffer[2] & 0x0003;
                ushTemp = ucBuffer[3] & 0x00FF;
                ushEncoderValue = ((ushEncoderValue << 8) & 0x0003 | ushTemp) & 0xFFFF;
            }
            iBufferFlag = 0;
            memset(ucBuffer, 0, 5);
        }
    }
}

unsigned short BrakeRead::getValue() {
    return ushEncoderValue;
}
