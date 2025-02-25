#include "types.h"

#ifndef RESPONSE_BUILDERS_H
#define RESPONSE_BUILDERS_H

int build_not_found_response(http_response *response);
int build_method_not_allowed_response(http_response *response);
int build_bad_request_response(http_response *response);
int build_ok_response(http_response *response);
int build_internal_server_error_response(http_response *res);
int fix_content_length(http_response *response);
int build_created_response(http_response *res);
int build_conflict_response(http_response *res);
int build_access_denied_response(http_response *res);

#endif