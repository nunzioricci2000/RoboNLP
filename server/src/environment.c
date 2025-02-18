#include <stdlib.h>
#include "utils.h"


inline int get_port() {
    char* port = getenv("PORT");
    if (port == NULL) {
        handle_error("Missing PORT in environment");
    }
    return atoi(port);
}