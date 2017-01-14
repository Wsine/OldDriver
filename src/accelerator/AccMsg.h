#ifndef _ACCMSG_H
#define _ACCMSG_H

struct AccMsg {
    bool toStop;
    int value;
    AccMsg(bool _toStop = true) : toStop(_toStop) {}
};

#endif