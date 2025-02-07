#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <pthread.h>

#include "server.h"

int main() {
    int socketFD;
    struct sockaddr_in socketAddress;

    server_socket_setup(&socketFD, &socketAddress);
    printf("Setup completed.\n");

    while (1) {
        printf("Now accepting a new connection.\n");
        pthread_t threadID;
        pthread_attr_t threadAttributes;

        struct destinationAddress destination;

        destination.socketFD = accept(socketFD,(struct sockaddr*) &(destination.address), &(destination.length));
        if (destination.socketFD == -1) {
            handle_error("accept");
        }

        pthread_attr_init(&threadAttributes);
        pthread_create(&threadID, &threadAttributes, accepting_tread, &destination);
    }
}