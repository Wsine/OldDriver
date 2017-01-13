#include "BrakeWrite.h"
#include "const.h"

BrakeWrite::BrakeWrite() {
    this->aim = this->current = BRAKE_LOOSE;
    this->isTurning = false;
}

BrakeWrite::~BrakeWrite() {
    Close();
}

void BrakeWrite::TurnDirection(int leftOrRight, int speed) {
    unsigned char buf[4];
    buf[0] = 0x80;
    buf[1] = (unsigned char)leftOrRight;// 0 or 1
    buf[2] = (unsigned char)speed;
    buf[3] = (unsigned char)((buf[0] + buf[1] + buf[2]) & 0x7f);
    Write(buf, 4);
}

void BrakeWrite::Stop() {
    unsigned char buf[4];
    buf[0] = 0x80;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = (unsigned char)((buf[0] + buf[1] + buf[2]) & 0x7f);
    Write(buf, 4);
}

void BrakeWrite::setAim(unsigned short _aim) {
    this->aim = _aim;
}

void BrakeWrite::setCurrent(unsigned short _current) {
    this->current = _current;
}

bool BrakeWrite::TurnToAim() {
    if (current > BRAKE_LOOSE && isTurning) {
        Stop();
        isTurning = false;
        return true;
    }

    if (current < BRAKE_STAMP && isTurning) {
        Stop();
        isTurning = false;
        return true;
    }

    if (current >= aim - BRAKE_MARGIN && current <= aim + BRAKE_MARGIN) {
        if (isTurning) {
            Stop();
        }
        isTurning = false;
        return true;
    } else if (current > aim + BRAKE_MARGIN) {
        // 左转
        if (!isTurning) {
            isTurning = true;
            TurnDirection(0, 80);
        }
    } else if (current < aim - BRAKE_MARGIN) {
        // 右转
        if (!isTurning) {
            isTurning = true;
            TurnDirection(1, 80);
        }
    }
    return false;
}

bool BrakeWrite::Reset() {
    this->aim = BRAKE_LOOSE;
    return TurnToAim();
}
