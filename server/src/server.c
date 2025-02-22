#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "server.h"
#include "utils.h"
#include "environment.h"

void server_socket_setup(int *socketFD, struct sockaddr_in *socketAddress) {
    *socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if ( *socketFD == -1 ) {
        handle_error("socket");
    }

    //Initialization
    memset(socketAddress, 0, sizeof(struct sockaddr_in));

    //Assignments
    socketAddress->sin_family = AF_INET;
    int port = get_port();
    socketAddress->sin_port = htons(port);
    socketAddress->sin_addr.s_addr = htonl(ADDRESS);
    
    socklen_t socketLength = sizeof(*socketAddress);
    if ( bind(*socketFD,(struct sockaddr *) socketAddress, socketLength) == -1 ) {
        handle_error("bind");
    }

    if ( listen(*socketFD, QUEUE_LENGTH) == -1 ) {
        handle_error("listen");
    }
}