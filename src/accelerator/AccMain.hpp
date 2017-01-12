#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <unistd.h>
#include "AccMsg.h"
#include "../const.h"

void accMain() {
    using namespace boost::interprocess;

    sleep(5);

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
                std::cout << "error handling message" << std::endl;
            }
            std::cout << msg.value << std::endl;
        }
    } catch (interprocess_exception& e) {
        std::cout << e.what() << std::endl;
    }
}