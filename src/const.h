#ifndef _CONST_H
#define _CONST_H

#include "uart/rpi_uart.h"

// Message Queue
const char *ACC_QUEUE_NAME = "acc_message_queue";
const int ACC_MSG_SIZE = 100;
const char *STEER_QUEUE_NAME = "steering_message_queue";
const int STEER_MSG_SIZE = 100;

// Control Mode
const int INIT_MODE = 0;
enum CONTROL_MODE {
    MANUAL_MODE = 0,
    AUTO_MODE = 1, 
    REMOTE_MODE = 2
};

// Serial Port
const char *ACC_SERIAL_PORT = "/dev/ttyUSB0";
const BAUDRATE ACC_BAUD_RATE = BR9600;

// Strange Value
const int ACC_INIT_VOLTAGE = 400;
const int ACC_OFF = 0;
const int ACC_ON = 1;

#endif