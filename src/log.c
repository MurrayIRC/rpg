#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void log_info(const char* msg, ...) {
    va_list args;
    va_start(args, msg);

    char result[100];
    strcpy(result, "[INFO]: ");
    strcat(result, msg);
    printf(result, args);
}

void log_warning(const char* msg, ...) {
    va_list args;
    va_start(args, msg);

    char result[100];
    strcpy(result, "[WARN]: ");
    strcat(result, msg);
    printf(result, args);
}

void log_error(const char* msg, ...) {
    va_list args;
    va_start(args, msg);

    char result[100];
    strcpy(result, "[ERR]: ");
    strcat(result, msg);
    printf(result, args);
}