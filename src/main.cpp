#include <boost/interprocess/ipc/message_queue.hpp>
#include <unistd.h>
#include <iostream>
#include "accelerator/AccMain.hpp"

int main(int argc, char **argv) {
    pid_t pid = fork();
    if (pid == 0) {
        // Parent Process
        using namespace boost::interprocess;
        try {
            // Erase previous message queue
            message_queue::remove(ACC_QUEUE_NAME);
            // Create a message queue
            message_queue mq(create_only, ACC_QUEUE_NAME, ACC_MSG_SIZE, sizeof(AccMsg));
            // Send Data
            AccMsg msg;
            for (int i = 0; i < ACC_MSG_SIZE; ++i) {
                msg.value = i;
                mq.send(&msg, sizeof(msg), 0);
                sleep(1);
            }
        } catch (interprocess_exception& e) {
            std::cout << e.what() << std::endl;
        }
    } else if (pid > 0) {
        // Children Process
        accMain();
    }
    return 0;
}