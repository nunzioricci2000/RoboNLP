#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include "types.h"
#include "picohttpparser.h"
#include "request_handler.h"

int recieve_request_head(int fd, http_request *request, char *buf, size_t buf_size);
int recieve_request_body(int fd, http_request *request, char *buf, size_t buf_size, size_t body_offset);
int recieve_request(int fd, http_request *request, char *buf, size_t buf_size);
int send_response(int fd, http_response *response);

void *request_handler(void *destinationSocketAddress) {
    fprintf(stderr, "DEBUG: Starting request_handler\n");
    int client_fd = *((int *)destinationSocketAddress);
    char buf[REQUEST_BUFFER_SIZE];
    char res_buf[RESPONSE_BUFFER_SIZE];
    http_request request;
    http_response response;

    fprintf(stderr, "DEBUG: Receiving request from client_fd %d\n", client_fd);
    int pret = recieve_request(client_fd, &request, buf, sizeof(buf));
    if (pret == -1) {
        fprintf(stderr, "DEBUG: Invalid request, preparing 400 response\n");
        response.status_code = 400;
        response.status_phrase = "Bad Request";
        response.num_headers = 0;
        response.body = NULL;
        response.body_len = 0;
    } else {
        fprintf(stderr, "DEBUG: Request received successfully\n");
        if(request.method_len == 3 && memcmp(request.method, "GET", 3) == 0) {
            fprintf(stderr, "DEBUG: Handle GET request\n");
            if(request.path_len == 1 && memcmp(request.path, "/", 1) == 0) {
                fprintf(stderr, "DEBUG: Sending default hello page\n");
                response.status_code = 200;
                response.status_phrase = "OK";
                response.num_headers = 1;
                response.headers[0].name = "Content-Type";
                response.headers[0].name_len = 12;
                response.headers[0].value = "text/html";
                response.headers[0].value_len = 9;
                response.body = "<html><body><h1>Hello, World!</h1></body></html>";
                response.body_len = 41;
            } else {
                fprintf(stderr, "DEBUG: Resource not found, sending 404\n");
                response.status_code = 404;
                response.status_phrase = "Not Found";
                response.num_headers = 0;
                response.body = NULL;
                response.body_len = 0;
            }
        } else {
            fprintf(stderr, "DEBUG: Method not allowed, sending 405\n");
            response.status_code = 405;
            response.status_phrase = "Method Not Allowed";
            response.num_headers = 0;
            response.body = NULL;
            response.body_len = 0;
        }
    }

    fprintf(stderr, "DEBUG: Sending response to client_fd %d\n", client_fd);
    send_response(client_fd, &response);
    close(client_fd);
    fprintf(stderr, "DEBUG: Finished handling request, closed client_fd %d\n", client_fd);
    return NULL;
}

int recieve_request(int fd, http_request *request, char *buf, size_t buf_size) {
    fprintf(stderr, "DEBUG: Entering recieve_request\n");
    request->num_headers = sizeof(request->headers)/sizeof(request->headers[0]);
    int ret = recieve_request_head(fd, request, buf, buf_size);
    if(ret == -1) {
        fprintf(stderr, "DEBUG: Error in recieve_request_head\n");
        return -1;
    }
    fprintf(stderr, "DEBUG: Request head received. Parsing body...\n");
    if(recieve_request_body(fd, request, buf, buf_size, ret) == -1) {
        fprintf(stderr, "DEBUG: Error in recieve_request_body\n");
        return -1;
    }
    fprintf(stderr, "DEBUG: Request body received.\n");
    return 0;
}

static int parse_request_head(const char *buf, size_t buf_len, http_request *request) {
    return phr_parse_request(
        buf, buf_len,
        &request->method, &request->method_len,
        &request->path, &request->path_len,
        &request->minor_version,
        request->headers, &request->num_headers, 0
    );
}

int recieve_request_head(int fd, http_request *request, char *buf, size_t buf_size) {
    fprintf(stderr, "DEBUG: Entering recieve_request_head\n");
    size_t buf_len = 0;
    while (1) {
        ssize_t rret = recv(fd, buf + buf_len, buf_size - buf_len, 0);
        if (rret <= 0) {
            fprintf(stderr, "DEBUG: Error or disconnect while reading head\n");
            return -1;
        }
        buf_len += rret;
        int pret = parse_request_head(buf, buf_len, request);
        if (pret > 0) {
            fprintf(stderr, "DEBUG: Request head fully parsed\n");
            return pret;
        }
        if (pret == -1) {
            fprintf(stderr, "DEBUG: Malformed request head\n");
            fprintf(stderr, "DEBUG: Buffer content: ");
            for (size_t i = 0; i < buf_len; i++) {
                switch (buf[i]) {
                    case '\n': fprintf(stderr, "\\n"); break;
                    case '\r': fprintf(stderr, "\\r"); break;
                    case '\t': fprintf(stderr, "\\t"); break;
                    case '\\': fprintf(stderr, "\\\\"); break;
                    default:
                        if (buf[i] >= 32 && buf[i] < 127) {
                            fprintf(stderr, "%c", buf[i]);
                        } else {
                            fprintf(stderr, "\\x%02x", (unsigned char)buf[i]);
                        }
                }
            }
            fprintf(stderr, "\n");
            return -1;
        }
        if (buf_len == buf_size) {
            fprintf(stderr, "DEBUG: Buffer full, but request not parsed\n");
            return -1;
        }
    }
}

static int extract_content_length(const http_request *request) {
    for (int i = 0; i < request->num_headers; i++) {
        if (request->headers[i].name_len == 14 &&
            strncasecmp(request->headers[i].name, "Content-Length", 14) == 0) {
            char clen[32] = {0};
            size_t len = request->headers[i].value_len < sizeof(clen)-1
                ? request->headers[i].value_len : sizeof(clen)-1;
            memcpy(clen, request->headers[i].value, len);
            return atoi(clen);
        }
    }
    return -1;
}

static ssize_t read_remaining_body(int fd, char *buf, size_t total_bytes, size_t remaining) {
    while (remaining > 0) {
        ssize_t rret = recv(fd, buf + total_bytes, remaining, 0);
        if (rret <= 0) {
            perror("recv body");
            return -1;
        }
        total_bytes += rret;
        remaining -= rret;
    }
    return total_bytes;
}

int recieve_request_body(int fd, http_request *request, char *buf, size_t buf_size, size_t body_offset) {
    fprintf(stderr, "DEBUG: Entering recieve_request_body\n");
    int content_length = extract_content_length(request);
    if (content_length < 0) {
        fprintf(stderr, "DEBUG: No body to read (Content-Length missing)\n");
        request->body = NULL;
        request->body_len = 0;
        return 0;
    }
    size_t already_read = buf_size > body_offset ? buf_size - body_offset : 0;
    if (already_read >= (size_t)content_length) {
        request->body = buf + body_offset;
        request->body_len = content_length;
        fprintf(stderr, "DEBUG: Body already in buffer\n");
        return 0;
    }
    size_t remaining = content_length - already_read;
    fprintf(stderr, "DEBUG: Need to read %zu more bytes of body\n", remaining);
    ssize_t total_bytes = read_remaining_body(fd, buf, buf_size, remaining);
    if (total_bytes < 0) {
        fprintf(stderr, "DEBUG: Error receiving remaining body\n");
        return -1;
    }
    request->body = buf + body_offset;
    request->body_len = content_length;
    fprintf(stderr, "DEBUG: Body read completely\n");
    return 0;
}

int send_response(int fd, http_response *response) {
    fprintf(stderr, "DEBUG: Entering send_response\n");
    char buf[RESPONSE_BUFFER_SIZE];
    size_t buf_len = 0;
    buf_len += snprintf(buf + buf_len, sizeof(buf) - buf_len, "HTTP/1.1 %d %s\r\n",
                        response->status_code, response->status_phrase);
    for(int i = 0; i < response->num_headers; i++) {
        buf_len += snprintf(buf + buf_len, sizeof(buf) - buf_len, "%.*s: %.*s\r\n",
                            (int)response->headers[i].name_len, response->headers[i].name,
                            (int)response->headers[i].value_len, response->headers[i].value);
    }
    buf_len += snprintf(buf + buf_len, sizeof(buf) - buf_len, "\r\n");
    ssize_t wret = send(fd, buf, buf_len, 0);
    if(wret == -1) {
        perror("send");
        return -1;
    }
    wret = send(fd, response->body, response->body_len, 0);
    if(wret == -1) {
        perror("send");
        return -1;
    }
    fprintf(stderr, "DEBUG: Response sent successfully\n");
    return 0;
}
