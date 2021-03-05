#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

typedef void (*log_lock_fn)(void* u_data, int lock);

enum { LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL };

#define log_info(...) log_dispatch(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warning(...) log_dispatch(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_dispatch(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_dispatch(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_set_u_data(void* u_data);
void log_set_loc(log_lock_fn fn);
void log_set_fp(FILE *fp);
void log_set_level(int level);
void log_set_quiet(int enable);

void log_dispatch(int level, const char *file, int line, const char *fmt, ...);

//void log_info(const char* msg, ...);
//void log_warning(const char* msg, ...);
//void log_error(const char* msg, ...);

#endif