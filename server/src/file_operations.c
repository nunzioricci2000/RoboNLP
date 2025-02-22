#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "types.h"

int is_valid_user(char *path) {
    return strlen(path) > 6 && access(path, F_OK) != -1;
}

int delete_user_file(char *path) {
    printf("Deleting file: %s\n", path);
    return unlink(path);
}

int get_user_file(char *path, char *buffer) {

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    printf("Reading file: %s\n", path);

    size_t read = fread(buffer, 1, MAX_RESPONSE_BODY_SIZE, file);
    fclose(file);
    if (read == 0) {
        return -1;
    } 

    size_t length = read<MAX_RESPONSE_BODY_SIZE?read:MAX_RESPONSE_BODY_SIZE;
    buffer[length] = '\0';

    printf("Read %zu bytes\nContent: \n%s\n", length, buffer);
    return length;
}

int post_user_file(char *path, char *content, size_t content_length) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }
    fwrite(content, 1, content_length, file);
    fclose(file);
}