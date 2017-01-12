#ifndef _STATUS_H_
#define _STATUS_H_
/*
 *  status > 0 : information
 *  status = 0 : success
 *  status < 0 : error
 *  000000 
 */

int debug(int status, char *file, int line);
#define SUCCESS 0
#define DEBUG(status) 
//debug(status, __FILE__,__LINE__)
 
/*/ uart_status*/
#define ERR_BASE_UART(X)        -1000+X

#define ERR_OPEN_COMPORT_FAILURE    ERR_BASE_UART(-001)
#define ERR_CLOSE_COMPORT_FAILURE   ERR_BASE_UART(-002)
#define ERR_WRITE_BYTE_FAILURE      ERR_BASE_UART(-003)
#define ERR_WRITE_STRING_FAILURE    ERR_BASE_UART(-004)
#define ERR_BAUDRATE_NOT_FOUND      ERR_BASE_UART(-005)
#define ERR_PARITY_NOT_FOUND        ERR_BASE_UART(-006)

#endif
