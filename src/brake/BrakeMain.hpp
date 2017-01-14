#include <boost/interprocess/ipc/message_queue.hpp>
#include <cstdio>
#include <unistd.h>
#include "const.h"
#include "BrakeMsg.h"
#include "BrakeRead.h"
#include "BrakeWrite.h"

static BrakeRead brakeRead;
static BrakeWrite brakeWrite;

void initBrake() {
    brakeRead.Open(BAK_R_SERIAL_PORT, BAK_R_BAUD_RATE);
    brakeWrite.Open(BAK_W_SERIAL_PORT, BAK_W_BAUD_RATE);
}

void handleMessage(const BrakeMsg& msg) {
    if (msg.toStop) {
        brakeWrite.Reset();
    } else {
        brakeWrite.setAim(msg.aim);
        brakeWrite.TurnToAim();
    }
    brakeRead.OnReceive();
    if (brakeRead.getValue() != 0) {
        brakeWrite.setCurrent(brakeRead.getValue());
    }
}

void brakeMain() {
    using namespace boost::interprocess;
    // Sleep and wait for creating message queue
    sleep(5);
    // Initial Brake
    initBrake();
    try {
        // open queue
        message_queue mq(open_only, BRAKE_QUEUE_NAME);
        // receive message
        size_t recvd_size;
        unsigned int priority;
        BrakeMsg msg, _msg;
        while (true) {
            if (mq.get_num_msg() > 0) {
                mq.receive((void*)&msg, sizeof(msg), recvd_size, priority);
                if (recvd_size != sizeof(msg)) {
                    printf("error handling steering message\n");
                } else {
                    _msg = msg;
                }
            }
            handleMessage(_msg);
        }
    } catch (interprocess_exception& e) {
        printf("%s\n", e.what());
    }
}
