#include <lcm/lcm-cpp.hpp>
#include <cstdio>
#include <unistd.h>
#include "communication/LcmHandler.h"
#include "steering/SteeringMain.hpp"
#include "accelerator/AccMain.hpp"
#include "brake/BrakeMain.hpp"
#include "communication/Controller.hpp"

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

void fork_for_brake() {
    pid_t pid = fork();
    if (pid == 0) {
        // Children Process
        brakeMain();
        exit(0);
    }
}

int main(int argc, char **argv) {

    lcm::LCM lcm;
    if (!lcm.good()) {
        printf("LCM is bad...\n");
        return 1;
    }

    fork_for_steering();
    fork_for_accelerator();
    fork_for_brake();

    // Parent Process
    LcmHandler handler;
    lcm.subscribe(LCM_CHANNEL_NAME, &LcmHandler::handleMessage, &handler);

    while (0 == lcm.handle());
    
    return 0;
}