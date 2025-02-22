#include "types.h"

#ifndef FILE_OPERATIONS
#define FILE_OPERATIONS
char* get_user_file(char *path, char *buffer);
int is_valid_user(char *path);
int delete_user_file(char *path);
int post_user_file(char *path, char *content, size_t content_length);
#endif