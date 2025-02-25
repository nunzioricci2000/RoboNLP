#ifndef TYPES_H
#define TYPES_H

#include <netinet/ip.h>
#include "picohttpparser.h"
#include "cJSON.h"

#define MAX_HEADER_NAME_LEN 128
#define MAX_HEADER_VALUE_LEN 4096
#define METHOD_LEN 10
#define PATH_LEN 100
#define MAX_HEADERS 100
#define MAX_REQUEST_BODY_SIZE 4096
#define MAX_RESPONSE_STATUS_PHRASE_LEN 128
#define MAX_RESPONSE_BODY_SIZE 4096

#define MAX_FACTS_LENGTH 2048
#define MAX_USERNAME_LENGTH 100
#define MAX_NAME_LENGTH 100
#define MAX_FIELD_NAME_LENGTH 100
#define FIELDS {"username", "name", "extraversion", "agreeableness", "conscientiousness", "emotional_stability", "openness_to_experience", "facts"}

// HTTP request and response structures
struct destination_address {
    int socketFD;
    struct sockaddr_in address;
    socklen_t length;
};

typedef struct {
    char name[MAX_HEADER_NAME_LEN];
    size_t name_len;
    char value[MAX_HEADER_VALUE_LEN];
    size_t value_len;
} http_header;

typedef struct {
    char method[METHOD_LEN];
    size_t method_len;
    char path[PATH_LEN];
    size_t path_len;
    int minor_version;
    http_header headers[MAX_HEADERS];
    size_t num_headers;
    char body[MAX_REQUEST_BODY_SIZE];
    size_t body_len;
} http_request;

typedef struct {
    int status_code;
    char status_phrase[MAX_RESPONSE_STATUS_PHRASE_LEN];
    size_t status_phrase_len;
    int minor_version;
    http_header headers[MAX_HEADERS];
    size_t num_headers;
    char body[MAX_RESPONSE_BODY_SIZE];
    size_t body_len;
} http_response;

// JSON parsing structures
typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    double extraversion;
    double agreeableness;
    double conscientiousness;
    double emotional_stability;
    double openness_to_experience;
    char facts[MAX_FACTS_LENGTH];
} user_profile;

#endif