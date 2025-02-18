void handle_error(const char *errorName) {
    perror(errorName);
    exit(-1);
}