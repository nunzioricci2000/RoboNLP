#include "types.h"

#ifndef FILE_OPERATIONS
#define FILE_OPERATIONS
int separate_path(const char *path, char username[], char field_name[]);
int get_user_file(char *username, char *buffer);
int get_field_from_user_file(char* field_name,char *username, char *buffer);
int is_valid_user(char *username);
int delete_user_file(char *username);
int delete_field_from_user_file(char* username, char* file_name);
int put_user_file(char *username, user_profile profile);
int post_user_file(user_profile profile);
int is_valid_field_name(char *filename);
int parse_user_profile(user_profile* profile, char* buf);
int post_user_facts(char* username, char* fact);
#endif