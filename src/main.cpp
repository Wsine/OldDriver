#include <boost/interprocess/ipc/message_queue.hpp>
#include <cstdio>
#include <unistd.h>
#include "accelerator/AccMain.hpp"
#include "steering/SteeringMain.hpp"

void fork_for_steering() {
    pid_t pid = fork();
    if (pid == 0) {
        // Children Process
        accMain();
        exit(0);
    }
}

void fork_for_accelerator() {
    pid_t pid = fork();
    if (pid == 0) {
        // Children Process
        steeringMain();
        exit(0);
    }
}

int main(int argc, char **argv) {
    fork_for_steering();
    fork_for_accelerator();

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
        printf("%s\n", e.what());
    }
    
    return 0;
}