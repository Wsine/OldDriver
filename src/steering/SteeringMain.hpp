#include <boost/interprocess/ipc/message_queue.hpp>
#include <cstdio>
#include <unistd.h>
#include "const.h"
#include "SteeringMsg.h"
#include "Steering.h"

static Steering steering;

void initSteering() {
    steering.StartDevice();
}

void handleMessage(const SteeringMsg& msg) {
    if (!msg.toStop) {
        steering.StartSelf_Driving(msg.angle, msg.speed);
    }
}

void steeringMain() {
    using namespace boost::interprocess;
    // Sleep and wait for creating message queue
    sleep(5);
    // Initial Steering
    initSteering();
    try {
        // open queue
        message_queue mq(open_only, STEER_QUEUE_NAME);
        // receive message
        size_t recvd_size;
        unsigned int priority;
        SteeringMsg msg, _msg;
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