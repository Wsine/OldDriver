#include <boost/interprocess/ipc/message_queue.hpp>
#include <cstdio>
#include <unistd.h>
#include "const.h"
#include "AccMsg.h"
#include "Accelerator.h"

static Accelerator accelerator;

void initAccelerator() {
    accelerator.Open(ACC_SERIAL_PORT, ACC_BAUD_RATE);
    accelerator.InputVoltage(ACC_INIT_VOLTAGE);
    accelerator.ShiftRelay(ACC_OFF);
}

void handleMessage(const AccMsg& msg) {
    if (msg.toStop) {
        if (accelerator.IsShiftRelayON()) {
            accelerator.InputVoltage(ACC_INIT_VOLTAGE);
            accelerator.ShiftRelay(ACC_OFF);
        }
    } else {
        if (!accelerator.IsShiftRelayON()) {
            accelerator.InputVoltage(ACC_INIT_VOLTAGE);
            accelerator.ShiftRelay(ACC_ON);
        }
        accelerator.InputVoltage(msg.value);
    }
}

void accMain() {
    using namespace boost::interprocess;
    // Sleep and wait for creating message queue
    sleep(5);
    // Initial Accelerator
    initAccelerator();
    try {
        // open queue
        message_queue mq(open_only, ACC_QUEUE_NAME);
        // receive message
        size_t recvd_size;
        unsigned int priority;
        AccMsg msg;
        while (true) {
            mq.receive((void*)&msg, sizeof(msg), recvd_size, priority);
            if (recvd_size != sizeof(msg)) {
                printf("error handling accelerator message\n");
            }
            handleMessage(msg);
        }
    } catch (interprocess_exception& e) {
        printf("%s\n", e.what());
    }
}