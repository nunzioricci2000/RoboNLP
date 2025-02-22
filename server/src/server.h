#ifndef SERVER
#define SERVER
#include <netinet/ip.h>

#define ADDRESS INADDR_ANY
#define QUEUE_LENGTH 50

void server_socket_setup(int *socketFD, struct sockaddr_in *socketAddress);
void *accepting_tread(void *destinationSocketAddress);

struct destinationAddress {
    int socketFD;
    struct sockaddr_in address;
    socklen_t length;
};
#endif