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
    char username[MAX_USERNAME_LENGTH];
    char field_name[MAX_FIELD_NAME_LENGTH];
    int path_type = separate_path(request->path, username, field_name);

    if (path_type >= 1) {
        if (!is_valid_user(username)) {
            build_not_found_response(response);
            return;
        }
    }

    if (path_type == 2) {
        if (!is_valid_field_name(field_name)) {
            build_not_found_response(response);
            return;
        }
    }

    int route = -1;

    if (strncmp(request->method, "GET", 3) == 0 && path_type > 0) {
        route = 0;
    } else if (strncmp(request->method, "POST", 4) == 0 && path_type == 0) {
        route = 1;
    } else if (strncmp(request->method, "DELETE", 6) == 0 && path_type > 0) {
        route = 2;
    } else if (strncmp(request->method, "PUT", 3) == 0 && path_type > 0) {
        route = 3;
    } else if (strncmp(request->method, "POST", 4) == 0 && path_type == 2 && strcmp(field_name, "facts") == 0) {
        route = 4;
    }
    else if (path_type == 0) {
        route = 5;
    } 


    
    user_profile profile;    
    switch (route) {
        case 0: //GET
            char buffer[MAX_RESPONSE_BODY_SIZE];
            if (path_type == 1) {
                if (get_user_file(username, buffer) < 0) {
                    build_internal_server_error_response(response);
                    break;
                }
            } else { //Path type must be 2 (it was checked in separate_path)
                if (get_field_from_user_file(field_name, username, buffer) < 0) {
                    build_internal_server_error_response(response);
                    break;
                }
            }
            build_ok_response(response);
            snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "%s" ,buffer);
            response->body_len = strlen(response->body);
            break;
        case 1: //POST
            if (path_type == 0) {
                if (parse_user_profile(&profile, request->body) < 0) {
                    build_bad_request_response(response);
                } else{
                    if (strcmp(profile.username, "") == 0 || strcmp(profile.name, "") == 0 ) {
                        build_bad_request_response(response);
                        printf("username (%s) or name (%s) were not found\n", profile.username, profile.name);
                    } else if (is_valid_user(profile.username)) {
                        build_conflict_response(response);
                    } else {
                        // TODO unparse user_profile and save it to a file named <username>
                        if ( post_user_file(profile) < 0) {
                            build_internal_server_error_response(response);
                        } else {
                            build_created_response(response);
                        }
                    }
                }
            }
            break;
        case 2: //DELETE
            char deleted[10] = "";
            if (path_type == 1) {
                if (delete_user_file(username) < 0) {
                    build_internal_server_error_response(response);
                    break;
                } else {
                    strcpy(deleted, "user");
                }
            } else if (strcmp(field_name, "username") == 0 || strcmp(field_name, "name") == 0) {
                build_bad_request_response(response);
                strcpy(response->body, "{\"error\": \"Cannot delete username or name\"}");
                response->body_len = strlen(response->body);
                break;
            } else { //Path type must be 2 (it was checked in separate_path) and the field is deletable
                if (delete_field_from_user_file(username, field_name) < 0) {
                    build_internal_server_error_response(response);
                    break;
                } else {
                    strcpy(deleted, field_name);
                }
            }
            build_ok_response(response);
            break;
        case 3: //PUT
            if (parse_user_profile(&profile, request->body) < 0) {
                build_bad_request_response(response);
            } else {
                if (strcmp(profile.username, "") != 0 && strcmp(profile.username, username) != 0 ) {
                    build_bad_request_response(response);
                    snprintf(response->body, MAX_RESPONSE_BODY_SIZE, "{\"error\": \"Username in URL does not match username in body (provided: %s, expected: %s)\"}", profile.username, username);
                    response->body_len = strlen(response->body);
                } else {
                    if ( put_user_file(username, profile) < 0) {
                        build_internal_server_error_response(response);
                    } else {
                        build_ok_response(response);
                    }
                }
            }
            break;
        case 4: //POST facts
            if (post_user_facts(username, request->body) < 0) {
                build_internal_server_error_response(response);
            } else {
                build_ok_response(response);
            }
            break;
        case 5: //User not found
            build_not_found_response(response);
            break;
        default:
            build_method_not_allowed_response(response);
            break;
    }
    
    fix_content_length(response);
}
