#include "types.h"

#ifndef FILE_OPERATIONS
#define FILE_OPERATIONS
char* get_user_file(char *username, char *buffer);
int is_valid_user(char *username);
int delete_user_file(char *username);
int put_user_file(user_profile profile);
int post_user_file(user_profile profile);
int is_valid_filename(char *filename);
int parse_user_profile(user_profile* profile, char* buf);
#endif