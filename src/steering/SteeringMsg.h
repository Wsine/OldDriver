#ifndef _STEERING_MSG_H
#define _STEERING_MSG_H

struct SteeringMsg {
    bool toStop;
    short angle;
    unsigned char speed;
    SteeringMsg(bool _toStop = true) : toStop(_toStop) {}
};

#endif