#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "log.h"

static struct {
    void* u_data;
    log_lock_fn lock;
    FILE *fp;
    int level;
    int quiet;
} log_data;

static const char* level_names[] = {
    "INFO", "WARNING", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
    "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

static void lock(void) {
    if (log_data.lock) {
        log_data.lock(log_data.u_data, 1);
    }
}

static void unlock(void) {
    if (log_data.lock) {
        log_data.lock(log_data.u_data, 0);
    }
}

void log_set_u_data(void* u_data) {
    log_data.u_data = u_data;
}

void log_set_lock(log_lock_fn fn) {
    log_data.lock = fn;
}

void log_set_fp(FILE* fp) {
    log_data.fp = fp;
}

void log_set_level(int level) {
    log_data.level = level;
}

void log_set_quiet(int enable) {
    log_data.quiet = enable ? 1 : 0;
}

void log_dispatch(int level, const char* file, int line, const char* fmt, ...) {
    if (level < log_data.level) {
        return;
    }

    lock();

    time_t t = time(NULL);
    #ifdef _WIN32
    struct tm l;
    struct tm *lt = &l;
    localtime_s(lt, &t);
    #else
    struct tm *lt = localtime(&t);
    #endif

    // Log to stderr
    if (!log_data.quiet) {
        va_list args;
        char buf[16];
        buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
    #ifdef LOG_USE_COLOR
        fprintf(stderr, "%s \x1b[90m%s:%d:\x1b[0m %s%-5s\x1b[0m", buf, file, line, level_colors[level], level_names[level]);
    #else
        fprintf(stderr, "%s %s:%d %-5s: ", buf, file, line, level_names[level]);
    #endif
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, "");
        fflush(stderr);
    }

    // Log to file
    if (log_data.fp) {
        va_list args;
        char buf[32];
        buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
        fprintf(log_data.fp, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
        va_start(args, fmt);
        vfprintf(log_data.fp, fmt, args);
        va_end(args);
        fprintf(log_data.fp, "\n");
        fflush(log_data.fp);
    }

    unlock();
}


/*void log_info(const char* msg, ...) {
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
}*/