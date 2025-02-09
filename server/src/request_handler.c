#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "types.h"
#include "picohttpparser.h"

void *request_handler(void *destinationSocketAddress) {
    struct destination_address* destination = (struct destination_address*)destinationSocketAddress;
    int fd = destination->socketFD;
    char buf[4096];
    ssize_t r = recv(fd, buf, sizeof(buf), 0);
    if (r <= 0) {
        close(fd);
        pthread_exit(NULL);
    }
    
    const char *method;
    size_t method_len;
    const char *path;
    size_t path_len;
    int minor_version;
    struct phr_header headers[100];
    size_t num_headers = sizeof(headers) / sizeof(headers[0]);
    
    int ret = phr_parse_request(buf, r, &method, &method_len, &path, &path_len,
                                &minor_version, headers, &num_headers, 0);
    if(ret < 0) {
        const char *error_response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        sendto(fd, error_response, strlen(error_response), MSG_NOSIGNAL,
               (struct sockaddr*) &(destination->address), destination->length);
        close(fd);
        pthread_exit(NULL);
    }
    
    char response_body[8192];
    int body_len = snprintf(response_body, sizeof(response_body),
        "Received Request:\nMethod: %.*s\nPath: %.*s\nHTTP version: 1.%d\nHeaders:\n",
        (int)method_len, method, (int)path_len, path, minor_version);
    for (size_t i = 0; i < num_headers; i++) {
        body_len += snprintf(response_body + body_len, sizeof(response_body) - body_len,
            "%.*s: %.*s\n",
            (int)headers[i].name_len, headers[i].name,
            (int)headers[i].value_len, headers[i].value);
    }
    
    char response_header[256];
    int header_len = snprintf(response_header, sizeof(response_header),
        "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nContent-Type: text/plain\r\n\r\n",
        body_len);
    
    sendto(fd, response_header, header_len, MSG_NOSIGNAL,
           (struct sockaddr*) &(destination->address), destination->length);
    sendto(fd, response_body, body_len, MSG_NOSIGNAL,
           (struct sockaddr*) &(destination->address), destination->length);
    
    close(fd);
    pthread_exit(NULL);
}