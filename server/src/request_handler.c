#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include "types.h"
#include "response_builders.h"
#include "picohttpparser.h"
#include "user_handler.h"
#include "request_handler.h"

int receive_request_head(int fd, http_request *request, char *buf, size_t buf_size);
int receive_request_body(int fd, http_request *request, char *buf, size_t buf_size, size_t body_offset);
int receive_request(int fd, http_request *request, char *buf, size_t buf_size);
int send_response(int fd, http_response *response);
static void free_http_response(http_response *response);

void *request_handler(void *destinationSocketAddress) {
    int client_fd = *((int *)destinationSocketAddress);
    char buf[REQUEST_BUFFER_SIZE];
    http_request request;
    http_response response;
    int pret = receive_request(client_fd, &request, buf, sizeof(buf));
    if (pret == -1) {
        build_bad_request_response(&response);
    } else {
        int route = -1;
        if (request.path_len == 1 && memcmp(request.path, "/", 1) == 0) {
            route = 0;
        } else if (request.path_len == 5 && memcmp(request.path, "/user", 5) == 0) {
            route = 1;
        }
        switch (route) {
            case 0:
                build_ok_response(&response);
                break;
            case 1:
                user_handler(&request, &response);
                break;
            default:
                build_not_found_response(&response);
                break;
        }
    }
    send_response(client_fd, &response);
    close(client_fd);
    return NULL;
}

int receive_request(int fd, http_request *request, char *buf, size_t buf_size) {
    request->num_headers = sizeof(request->headers)/sizeof(request->headers[0]);
    int ret = receive_request_head(fd, request, buf, buf_size);
    if(ret == -1) {
        return -1;
    }
    if(receive_request_body(fd, request, buf, buf_size, ret) == -1) {
        return -1;
    }
    return 0;
}

static int parse_request_head(const char *buf, size_t buf_len, http_request *request) {
    const char *method_ptr;
    size_t method_len;
    const char *path_ptr;
    size_t path_len;
    int minor_version;
    struct phr_header tmp_headers[MAX_HEADERS];
    size_t tmp_num_headers = MAX_HEADERS;
    
    int ret = phr_parse_request(
        buf, buf_len,
        &method_ptr, &method_len,
        &path_ptr, &path_len,
        &minor_version,
        tmp_headers, &tmp_num_headers, 0
    );
    if (ret > 0) {
        size_t copy_method = method_len < METHOD_LEN - 1 ? method_len : METHOD_LEN - 1;
        memcpy(request->method, method_ptr, copy_method);
        request->method[copy_method] = '\0';
        request->method_len = copy_method;
        size_t copy_path = path_len < PATH_LEN - 1 ? path_len : PATH_LEN - 1;
        memcpy(request->path, path_ptr, copy_path);
        request->path[copy_path] = '\0';
        request->path_len = copy_path;
        request->num_headers = 0;
        for (size_t i = 0; i < tmp_num_headers && i < MAX_HEADERS; i++) {
            size_t copy_name = tmp_headers[i].name_len < MAX_HEADER_NAME_LEN - 1 ? tmp_headers[i].name_len : MAX_HEADER_NAME_LEN - 1;
            memcpy(request->headers[i].name, tmp_headers[i].name, copy_name);
            request->headers[i].name[copy_name] = '\0';
            request->headers[i].name_len = copy_name;
            size_t copy_value = tmp_headers[i].value_len < MAX_HEADER_VALUE_LEN - 1 ? tmp_headers[i].value_len : MAX_HEADER_VALUE_LEN - 1;
            memcpy(request->headers[i].value, tmp_headers[i].value, copy_value);
            request->headers[i].value[copy_value] = '\0';
            request->headers[i].value_len = copy_value;
            request->num_headers++;
        }
        request->minor_version = minor_version;
    }
    return ret;
}

int receive_request_head(int fd, http_request *request, char *buf, size_t buf_size) {
    size_t buf_len = 0;
    while (1) {
        ssize_t rret = recv(fd, buf + buf_len, buf_size - buf_len, 0);
        if (rret <= 0) {
            return -1;
        }
        buf_len += rret;
        int pret = parse_request_head(buf, buf_len, request);
        if (pret > 0) {
            return pret;
        }
        if (pret == -1) {
            return -1;
        }
        if (buf_len == buf_size) {
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

int receive_request_body(int fd, http_request *request, char *buf, size_t buf_size, size_t body_offset) {
    int content_length = extract_content_length(request);
    if (content_length < 0) {
        request->body_len = 0;
        return 0;
    }
    size_t already_read = buf_size > body_offset ? buf_size - body_offset : 0;
    if (already_read >= (size_t)content_length) {
        memcpy(request->body, buf + body_offset, content_length);
        request->body_len = content_length;
        return 0;
    }
    size_t remaining = content_length - already_read;
    ssize_t total_bytes = read_remaining_body(fd, buf, buf_size, remaining);
    if (total_bytes < 0) {
        return -1;
    }
    memcpy(request->body, buf + body_offset, content_length);
    request->body_len = content_length;
    return 0;
}

int send_response(int fd, http_response *response) {
    char header[RESPONSE_BUFFER_SIZE];
    size_t header_len = 0;
    header_len += snprintf(header + header_len, sizeof(header) - header_len,
                             "HTTP/1.1 %d %s\r\n", response->status_code, response->status_phrase);
    for (int i = 0; i < response->num_headers; i++) {
        header_len += snprintf(header + header_len, sizeof(header) - header_len,
                                 "%.*s: %.*s\r\n",
                                 (int)response->headers[i].name_len, response->headers[i].name,
                                 (int)response->headers[i].value_len, response->headers[i].value);
    }
    header_len += snprintf(header + header_len, sizeof(header) - header_len, "\r\n");

    size_t total_len = header_len + response->body_len;
    char *send_buf = malloc(total_len);
    if (send_buf == NULL) {
        perror("malloc");
        return -1;
    }
    memcpy(send_buf, header, header_len);
    memcpy(send_buf + header_len, response->body, response->body_len);

    ssize_t wret = send(fd, send_buf, total_len, 0);
    free(send_buf);
    if (wret == -1) {
        perror("send");
        return -1;
    }
    return 0;
}
