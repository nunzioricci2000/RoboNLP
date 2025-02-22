#include <stdlib.h>  
#include <stdio.h>
#include <errno.h>
void handle_error(const char *errorName) {
    perror(errorName);
    exit(-1);
}