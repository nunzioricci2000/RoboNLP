#include "user_handler.h"
#include "response_builders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "file_operations.h"
#include "utils.h"

void user_handler(http_request *request, http_response *response) {
    const int user_exists = is_valid_user(request->path);

    int route = -1;

    if (strncmp(request->method, "GET", 3) == 0 && user_exists) {
        route = 0;
    } else if (strncmp(request->method, "POST", 4) == 0) {
        route = 1;
    } else if (strncmp(request->method, "DELETE", 6) == 0 && user_exists) {
        route = 2;
    } else if (!user_exists) {
        route = 3;
    } 

    switch (route) {
        case 0: //GET
            char buffer[MAX_RESPONSE_BODY_SIZE];
            if (get_user_file(request->path, buffer) > 0) {
                build_ok_response(response);
                snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "%s" ,buffer);
                response->body_len = strlen(response->body);
            } else {
                build_internal_server_error_response(response);
            }
            break;
        case 1: //POST
            if (!user_exists) {
                if ( post_user_file(request->path, request->body, request->body_len) < 0) {
                    build_internal_server_error_response(response);
                } else {
                    build_created_response(response);
                    snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "User created successfully");
                }
                 
            } else {
                build_conflict_response(response);
            }
            break;
        case 2: //DELETE
            if (delete_user_file(request->path) == 0) {
                build_ok_response(response);
                snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "User deleted successfully");
            } else { 
                handle_error("unlink");
                build_internal_server_error_response(response);
            }
            break;
        case 3: //User not found
            build_not_found_response(response);
            break;
        default:
            build_method_not_allowed_response(response);
            break;
    }
    
    fix_content_length(response);
}
