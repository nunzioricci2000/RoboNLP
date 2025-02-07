void server_socket_setup(int *socketFD, struct sockaddr_in *socketAddress);
void handle_error(const char *errorName);
void *accepting_tread(void *destinationSocketAddress);

struct destinationAddress {
    int socketFD;
    struct sockaddr_in address;
    socklen_t length;
};