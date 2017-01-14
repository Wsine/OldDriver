#ifndef _CONST_H
#define _CONST_H

#include "uart/rpi_uart.h"

// Message Queue
static const char *ACC_QUEUE_NAME = "acc_message_queue";
static const int ACC_MSG_SIZE = 100;
static const char *STEER_QUEUE_NAME = "steering_message_queue";
static const int STEER_MSG_SIZE = 100;
static const char *BRAKE_QUEUE_NAME = "brake_message_queue";
static const int BRAKE_MSG_SIZE = 100;

// Control Mode
static const int INIT_MODE = 0;
enum CONTROL_MODE {
    MANUAL_MODE = 0,
    AUTO_MODE = 1, 
    REMOTE_MODE = 2
};

// Serial Port
static const char *ACC_SERIAL_PORT = "/dev/ttyUSB0";
static const BAUDRATE ACC_BAUD_RATE = BR9600;
static const char *BAK_R_SERIAL_PORT = "/dev/ttyUSB1";
static const BAUDRATE BAK_R_BAUD_RATE = BR38400;
static const char *BAK_W_SERIAL_PORT = "/dev/ttyUSB2";
static const BAUDRATE BAK_W_BAUD_RATE = BR38400;

// Strange Value
static const int ACC_INIT_VOLTAGE = 400;
static const int ACC_OFF = 0;
static const int ACC_ON = 1;
static const unsigned short BRAKE_LOOSE = 900;
static const unsigned short BRAKE_STAMP = 540;
static const unsigned short BRAKE_MARGIN = 15;

#endif
