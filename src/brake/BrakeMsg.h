#ifndef _BRAKE_MSG_H
#define _BRAKE_MSG_H

struct BrakeMsg {
    bool toStop;
    unsigned short aim;
    BrakeMsg(bool _toStop = true) : toStop(_toStop) {}
};

#endif
