#include "LcmHandler.h"
#include "accelerator/AccMsg.h"
#include "brake/BrakeMsg.h"
#include "steering/SteeringMsg.h"
#include "const.h"

LcmHandler::LcmHandler() {
    // Erase previous message queue if exists
    message_queue::remove(STEER_QUEUE_NAME);
    message_queue::remove(ACC_QUEUE_NAME);
    message_queue::remove(BRAKE_QUEUE_NAME);
    // Create message queues
    steerMQ = new message_queue(create_only, STEER_QUEUE_NAME, STEER_MSG_SIZE, sizeof(SteeringMsg));
    accMQ = new message_queue(create_only, ACC_QUEUE_NAME, ACC_MSG_SIZE, sizeof(AccMsg));
    brakeMQ = new message_queue(create_only, BRAKE_QUEUE_NAME, BRAKE_MSG_SIZE, sizeof(BrakeMsg));
}

LcmHandler::~LcmHandler() {
    delete(steerMQ);
    delete(accMQ);
    delete(brakeMQ);
    // Erase message queue
    message_queue::remove(STEER_QUEUE_NAME);
    message_queue::remove(ACC_QUEUE_NAME);
    message_queue::remove(BRAKE_QUEUE_NAME);
}

void LcmHandler::handleMessage(const lcm::ReceiveBuffer *rbuf,
        const std::string& chan, const Controller *ctrl) {

    try {
        // Send Data
        if (ctrl->steering_enabled) {
            SteeringMsg steering_msq(false);
            steering_msq.angle = (short)ctrl->steering_angle;
            steering_msq.speed = (unsigned char)ctrl->steering_speed;
            steerMQ->try_send(&steering_msq, sizeof(steering_msq), 0);
        }

        if (ctrl->accelerator_enabled) {
            AccMsg acc_msg(false);
            acc_msg.value = (int)ctrl->accelerator_value;
            accMQ->try_send(&acc_msg, sizeof(acc_msg), 0);
        }

        if (ctrl->brake_enabled) {
            BrakeMsg brake_msg(false);
            if (ctrl->current_speed <= ctrl->target_speed) {
                brake_msg.aim = BRAKE_LOOSE;
            } else if (ctrl->target_speed == 0) {
                brake_msg.aim = BRAKE_STAMP;
            } else {
                brake_msg.aim = calcBrakeAim(ctrl->current_speed, ctrl->target_speed);
            }
            brakeMQ->try_send(&brake_msg, sizeof(brake_msg), 0);
        }
    } catch (interprocess_exception& e) {
        printf("%s\n", e.what());
    }
    
}

// Note cur must be larger than tar
unsigned short LcmHandler::calcBrakeAim(const unsigned short& cur,
        const unsigned short& tar) {
    float n = (cur - tar) / BRAKE_CALC_FACTOR;
    if (n >= BRAKE_CALC_MAX) {
        return BRAKE_STAMP;
    }
    const short diff = BRAKE_STAMP - BRAKE_LOOSE;
    return (unsigned short)(diff / n);
}