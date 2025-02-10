#include <stdio.h>
#include <string.h>
#include "response_builders.h"

static void set_status(http_response *res, int code, const char *phrase) {
    res->status_code = code;
    strncpy(res->status_phrase, phrase, MAX_RESPONSE_STATUS_PHRASE_LEN - 1);
    res->status_phrase_len = strlen(phrase);
    res->minor_version = 1;
}

static void add_header(http_response *res, const char *name, const char *value, size_t index) {
    strncpy(res->headers[index].name, name, MAX_HEADER_NAME_LEN - 1);
    res->headers[index].name_len = strlen(name);
    strncpy(res->headers[index].value, value, MAX_HEADER_VALUE_LEN - 1);
    res->headers[index].value_len = strlen(value);
}

static void set_body(http_response *res, const char *content) {
    size_t content_length = strlen(content);
    strncpy(res->body, content, MAX_RESPONSE_BODY_SIZE - 1);
    res->body_len = content_length;
}

int fix_content_length(http_response *res) {
    for (int i = 0; i < res->num_headers; i++) {
        if (strncmp(res->headers[i].name, "Content-Length", MAX_HEADER_NAME_LEN) == 0) {
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%zu", res->body_len);
            strncpy(res->headers[i].value, buffer, MAX_HEADER_VALUE_LEN - 1);
            res->headers[i].value_len = strlen(buffer);
            return 0;
        }
    }
    return -1;
}

static int build_response(http_response *res, int code, const char *phrase, const char *content) {
    char buffer[20];
    size_t content_length = strlen(content);

    set_status(res, code, phrase);

    add_header(res, "Content-Type", "text/html", 0);
    snprintf(buffer, sizeof(buffer), "%zu", content_length);
    add_header(res, "Content-Length", buffer, 1);
    res->num_headers = 2;

    set_body(res, content);

    fix_content_length(res);

    return 0;
}

int build_ok_response(http_response *res) {
    return build_response(res, 200, "OK", "<html><body><h1>200 OK</h1></body></html>");
}

int build_not_found_response(http_response *res) {
    return build_response(res, 404, "Not Found", "<html><body><h1>404 Not Found</h1></body></html>");
}

int build_method_not_allowed_response(http_response *res) {
    return build_response(res, 405, "Method Not Allowed", "<html><body><h1>405 Method Not Allowed</h1></body></html>");
}

int build_bad_request_response(http_response *res) {
    return build_response(res, 400, "Bad Request", "<html><body><h1>400 Bad Request</h1></body></html>");
}
