#ifndef _RPI_UART_H
#define _RPI_UART_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "uart_status.h"

typedef enum _PARITY {
    P_8N1, /* No parity (8N1)   */
    P_7E1, /* Even parity (7E1)*/
    P_7O1, /* Odd parity (7O1)  */
    P_7S1 /* Space parity is setup the same as no parity (7S1)  */
} PARITY;

typedef enum _BAUDRATE {
    BR2400,
    BR4800,
    BR9600,
    BR19200,
    BR38400,
    BR57600,
    BR115200
} BAUDRATE;

class rpi_uart {
private:
    int fd;

public:
    rpi_uart(void) {
        //"/dev/ttyUSB0"
    }

    int Open(const char* comPort, BAUDRATE baud_rate) {
        struct termios options;

        int status;
        /*  O_NOCTTY = doesn't want to be the "controlling terminal" */
        fd = open(comPort, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);

        if (fd < 0) {
            return -1;
        } else {
            fcntl(fd, F_SETFL, 0);
        }

        if (0 == isatty(STDIN_FILENO)) {
            printf("standard input is not a terminal device\n");
            return -1;
        } else {
            printf("is tty device!\n");
        }

        if (tcgetattr(fd, &options) != 0) {
            printf("SetupSerial 1\n");
        }

        /*  set baud rate */
        if ((status = setBaudRate(&options, baud_rate)) < 0)
            return -1;

        /*  set parity  */
        if ((status = setParity(&options, P_8N1)) < 0)
            return -1;
        return 0;
    }

    bool is_uart_ok(void) {
        if (fd < 0)
            return true;
        else
            return false;
    }

    int closeComPort(void) {
        if (close(fd)) {
            return (ERR_CLOSE_COMPORT_FAILURE);
        }
        return SUCCESS;
    }

    int writeByte(unsigned char byte) {
        int n = write(fd, &byte, 1);
        return (n < 0) ? (ERR_WRITE_BYTE_FAILURE) : n;
    }

    int writeBytes(char* bytes, size_t len) {
        int n = write(fd, bytes, len);
        return (n < 0) ? (ERR_WRITE_BYTE_FAILURE) : n;
    }

    int Write(unsigned char* bytes, int len) {
        int _len = 0;
        _len = write(fd, bytes, len);
        if (_len == len) {
            return len;
        } else {
            tcflush(fd, TCOFLUSH);
            return 0;
        }
    }

    void Close(void) {
        if (fd > 0)
            close(fd);
    }

    int Read(unsigned char* bytes, int len) {
        int _len = 0, fs_sel, cnt = 0;
        fd_set fs_read;

        struct timeval time;

        FD_ZERO(&fs_read);
        FD_SET(fd, &fs_read);

        time.tv_sec = 0;
        time.tv_usec = 500000;

        while (_len != len) {
            //使用select实现串口的多路通信
            fs_sel = select(fd + 1, &fs_read, NULL, NULL, &time);
            if (fs_sel) {
                cnt = read(fd, bytes, len - _len);
                bytes += cnt;
            } else {
                break;
            }
            _len += cnt;
            cnt = 0;
        }
        return _len;
    }

    int ReadString(unsigned char* bytes, int len) {
        bytes[Read(bytes, len - 1)] = '\0';
    }

    int readByte(char* byte) {
        int n = read(fd, &byte[0], 1);
        return (n < 0) ? (ERR_WRITE_BYTE_FAILURE) : n;
    }

    int readBytes(char* byte, size_t len) {
        int n = read(fd, byte, len);
        return (n < 0) ? (ERR_WRITE_BYTE_FAILURE) : n;
    }

    int setBaudRate(struct termios* options, BAUDRATE baud_rate) {
        /* SET BOUD RATE */
        switch (baud_rate) {
        case BR2400:
            cfsetispeed(options, B2400);
            cfsetospeed(options, B2400);
            break;
        case BR4800:
            cfsetispeed(options, B4800);
            cfsetospeed(options, B4800);
            break;
        case BR9600:
            cfsetispeed(options, B9600);
            cfsetospeed(options, B9600);
            break;
        case BR19200:
            cfsetispeed(options, B19200);
            cfsetospeed(options, B19200);
            break;
        case BR38400:
            cfsetispeed(options, B38400);
            cfsetospeed(options, B38400);
            break;
        case BR57600:
            cfsetispeed(options, B57600);
            cfsetospeed(options, B57600);
            break;
        case BR115200:
            cfsetispeed(options, B115200);
            cfsetospeed(options, B115200);
            break;
        default:
            return (ERR_BAUDRATE_NOT_FOUND);
        }

        return SUCCESS;
    }

    int setParity(struct termios* options, PARITY parity) {

        /*  Enable the receiver and set local mode...   */
        options->c_cflag |= (CLOCAL | CREAD);

        /*  Disable hardware flow   */
        options->c_cflag &= ~CRTSCTS;

        /*  Disable software flow   */
        options->c_iflag &= ~(IXON | IXOFF | IXANY);

        /*  Enable non-canonical    */
        options->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

        options->c_oflag &= ~OPOST;

        /*  Time to wait for data   */
        options->c_cc[VTIME] = 0;

        /*  Minimum number of characters to read    */
        options->c_cc[VMIN] = 1;

        switch (parity) {
        case P_8N1:
            /*  No parity (8N1) */
            options->c_cflag &= ~PARENB;
            options->c_cflag &= ~CSTOPB;
            options->c_cflag &= ~CSIZE;
            options->c_cflag |= CS8;
            break;
        case P_7E1:
            /*  Even parity (7E1)   */
            options->c_cflag |= PARENB;
            options->c_cflag &= ~PARODD;
            options->c_cflag &= ~CSTOPB;
            options->c_cflag &= ~CSIZE;
            options->c_cflag |= CS7;
            break;
        case P_7O1:
            /* Odd parity (7O1) */
            options->c_cflag |= PARENB;
            options->c_cflag |= PARODD;
            options->c_cflag &= ~CSTOPB;
            options->c_cflag &= ~CSIZE;
            options->c_cflag |= CS7;
            break;
        case P_7S1:
            /* Space parity is setup the same as no parity (7S1)    */
            options->c_cflag &= ~PARENB;
            options->c_cflag &= ~CSTOPB;
            options->c_cflag &= ~CSIZE;
            options->c_cflag |= CS8;
            break;
        default:
            return (ERR_PARITY_NOT_FOUND);
        }

        /*  Set the new options for the port... */
        tcsetattr(fd, TCSANOW, options);

        return SUCCESS;
    }

    //Write();
    //Read();
};

#endif //! _RPI_UART_H_
