#include <stdio.h>

int debug(int status, char *file, int line) {
    printf("code=%08d,file=%s,line=%d\n",status, file, line);   
    return status;
}