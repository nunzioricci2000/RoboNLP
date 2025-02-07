#include <netinet/ip.h>

#define PORT 1025
#define ADDRESS INADDR_LOOPBACK
#define QUEUE_LENGHT 50

void server_socket_setup(int *socketFD, struct sockaddr_in *socketAddress);
void handle_error(const char *errorName);
void *accepting_tread(void *destinationSocketAddress);
