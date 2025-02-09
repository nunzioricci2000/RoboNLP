#include <netinet/ip.h>

#ifndef TYPES_H
#define TYPES_H

struct destination_address {
    int socketFD;
    struct sockaddr_in address;
    socklen_t length;
};

#endif