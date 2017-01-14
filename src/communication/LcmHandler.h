#ifndef _LCM_HANDLER_H
#define _LCM_HANDLER_H

#include <boost/interprocess/ipc/message_queue.hpp>
#include <lcm/lcm-cpp.hpp>
#include "Controller.hpp"

using namespace boost::interprocess;

class LcmHandler {
private:
    message_queue *accMQ;
    message_queue *brakeMQ;
    message_queue *steerMQ;
public:
    LcmHandler();
    ~LcmHandler();
    void handleMessage(const lcm::ReceiveBuffer *rbuf,
        const std::string& chan,
        const Controller *ctrl);
    unsigned short calcBrakeAim(const unsigned short& cur,
        const unsigned short& tar);
};

#endif
