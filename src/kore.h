/* credit for this module lies entirely with Eskil Steenberg. Some of his code has been adapted here for my purposes, with some minor convenience edits. see http://gamepipeline.org/ for more. */

#ifndef KORE_H
#define	KORE_H

#ifndef NULL
#ifdef __cplusplus
#define NULL    0 /* Defines NULL in C++*/
#else
#define NULL    ((void *)0) /* Defines NULL in C*/
#endif
#endif

#ifndef TRUE
#define TRUE 1 /* Defines TRUE */
#endif
#ifndef FALSE
#define FALSE 0 /* Defines FALSE*/
#endif
#ifdef _WIN32
typedef unsigned int uint;
typedef signed __int64 int64;
typedef unsigned __int64 uint64;
#else
#include <sys/types.h>
#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef unsigned char boolean;

/* Defines PI */
#define PI  3.1415926535897932384626433832795028841971693993751058209749445923

#ifndef NO_MEMORY_DEBUG
#define MEMORY_DEBUG /* turns on the memory debugging system */
#endif
#ifndef EXIT_CRASH
#define EXIT_CRASH /* turns on crash on exit */
#endif

#ifdef MEMORY_DEBUG
/* ----- Debugging -----
If MEMORY_DEBUG  is enabled, the memory debugging system will create macros that replace malloc, free and realloc and allows the system to kept track of and report where memory is being allocated, how much and if the memory is being fred. This is very useful for finding memory leaks in large applications. The system can also over allocate memory and fill it with a magic number and can therfor detect if the application writes outside of the allocated memory. if EXIT_CRASH is defined, then exit(); will be replaced with a funtion that writes to NULL. This will make it trivial ti find out where an application exits using any debugger., */

extern void debug_memory_init(void (*lock)(void *mutex), void (*unlock)(void *mutex), void *mutex); /* Required for memory debugger to be thread safe */
extern void *debug_mem_malloc(uint size, char *file, uint line); /* Replaces malloc and records the c file and line where it was called*/
extern void *debug_mem_realloc(void *pointer, uint size, char *file, uint line); /* Replaces realloc and records the c file and line where it was called*/
extern void debug_mem_free(void *buf); /* Replaces free and records the c file and line where it was called*/
extern void debug_mem_print(uint min_allocs); /* Prints out a list of all allocations made, their location, how much memorey each has allocated, freed, and how many allocations have been made. The min_allocs parameter can be set to avoid printing any allocations that have been made fewer times then min_allocs */
extern void debug_mem_reset(void); /* f_debug_mem_reset allows you to clear all memory stored in the debugging system if you only want to record allocations after a specific point in your code*/
extern boolean debug_memory(void); /*f_debug_memory checks if any of the bounds of any allocation has been over written and reports where to standard out. The function returns TRUE if any error was found*/

#define malloc(n) debug_mem_malloc(n, __FILE__, __LINE__) /* Replaces malloc. */
#define realloc(n, m) debug_mem_realloc(n, m, __FILE__, __LINE__) /* Replaces realloc. */
#define free(n) debug_mem_free(n) /* Replaces free. */

#endif

#ifdef EXIT_CRASH
extern void exit_crash(uint i); /* function guaranteed to crash (Writes to NULL).*/
#define exit(n) exit_crash(n) /* overwriting exit(0) with a function guaraneed to crash. */
#endif

#endif