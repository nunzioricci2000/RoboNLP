#ifndef TYPES_H
#define TYPES_H

#include <netinet/ip.h>
#include "picohttpparser.h"

struct destination_address {
    int socketFD;
    struct sockaddr_in address;
    socklen_t length;
};

typedef struct {
    const char *method;
    size_t method_len;
    const char *path;
    size_t path_len;
    int minor_version;
    struct phr_header headers[100];
    size_t num_headers;
    const char *body;
    size_t body_len;
} http_request;

typedef struct {
    int status_code;
    const char *status_phrase;
    size_t status_phrase_len;
    int minor_version;
    struct phr_header headers[100];
    size_t num_headers;
    const char *body;
    size_t body_len;
} http_response;

#endif