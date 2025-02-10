#include <stdlib.h>
#include "response_builders.h"

int build_not_found_response(http_response *response) {
    response->status_code = 404;
    response->status_phrase = "Not Found";
    response->num_headers = 0;
    response->body = NULL;
    response->body_len = 0;
    return 0;
}

int build_method_not_allowed_response(http_response *response) {
    response->status_code = 405;
    response->status_phrase = "Method Not Allowed";
    response->num_headers = 0;
    response->body = NULL;
    response->body_len = 0;
    return 0;
}

int build_bad_request_response(http_response *response) {
    response->status_code = 400;
    response->status_phrase = "Bad Request";
    response->num_headers = 0;
    response->body = NULL;
    response->body_len = 0;
    return 0;
}

int build_ok_response(http_response *response) {
    response->status_code = 200;
    response->status_phrase = "OK";
    response->num_headers = 0;
    response->body = NULL;
    response->body_len = 0;
    return 0;
}