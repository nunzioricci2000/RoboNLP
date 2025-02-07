#include <unistd.h>
#include <pthread.h>
#include "types.h"

void *request_handler(void *destinationSocketAddress) {
    struct destination_address* destination = (struct destination_address*) destinationSocketAddress;

    // TODO: Edit to behave as needed
    sendto(destination->socketFD, "Test message\n", 30, MSG_NOSIGNAL, (struct sockaddr*) &(destination->address), (destination->length));
    
    close(destination->socketFD);
    pthread_exit(0);
}