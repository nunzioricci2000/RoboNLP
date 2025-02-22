#include "user_handler.h"
#include "response_builders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void user_handler(http_request *request, http_response *response) {
    if (strncmp(request->method, "GET", 3) == 0) {
        build_ok_response(response);
        const char body[] = "[\"Alice\", \"Bob\", \"Charlie\"]";
        snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "%s", body);
        response->body_len = strlen(response->body);
    } else if (strncmp(request->method, "POST", 4) == 0) {
        response->status_code = 201;
        snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "User created successfully");
    } else {
        response->status_code = 405;
        snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "Method Not Allowed");
        response->body[MAX_RESPONSE_BODY_SIZE - 1] = '\0';
        response->body_len = strlen(response->body) + 1;
    }
    fix_content_length(response);
}
