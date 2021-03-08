#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NO_MEMORY_DEBUG
#include "kore.h"

extern void debug_mem_print(uint min_allocs);

#define MEMORY_OVER_ALLOC 32
#define MEMORY_MAGIC_NUMBER 132
typedef struct{
    uint size;
    void *buf;
} STMemAllocBuf;

typedef struct{
    uint line;
    char file[256];
    STMemAllocBuf *allocs;
    uint alloc_count;
    uint alloc_alocated;
    uint size;
    uint alocated;
    uint freed;
} STMemAllocLine;

STMemAllocLine alloc_lines[1024];
uint alloc_line_count = 0;
void *alloc_mutex = NULL;
void (*alloc_mutex_lock)(void *mutex) = NULL;
void (*alloc_mutex_unlock)(void *mutex) = NULL;

void debug_memory_init(void (*lock)(void *mutex), void (*unlock)(void *mutex), void *mutex) {
    alloc_mutex = mutex;
    alloc_mutex_lock = lock;
    alloc_mutex_unlock = unlock;
}

boolean debug_memory(void) {
    boolean output = FALSE;
    uint i, j, k;
    if (alloc_mutex != NULL)
        alloc_mutex_lock(alloc_mutex);
    for (i = 0; i < alloc_line_count; i++) {
        for (j = 0; j < alloc_lines[i].alloc_count; j++) {
            uint8 *buf;
            uint size;
            buf = alloc_lines[i].allocs[j].buf;
            size = alloc_lines[i].allocs[j].size;
            for (k = 0; k < MEMORY_OVER_ALLOC; k++) {
                if (buf[size + k] != MEMORY_MAGIC_NUMBER) {
                    break;
                }
            }
            if (k < MEMORY_OVER_ALLOC) {
                printf("MEM ERROR: Overshoot at line %u in file %s\n", alloc_lines[i].line, alloc_lines[i].file);
                
                {
                    uint *X = NULL;
                    X[0] = 0;
                }

                output = TRUE;
            }
        }
    }
    if (alloc_mutex != NULL) {
        alloc_mutex_unlock(alloc_mutex);
    }
    return output;
}

void debug_mem_add(void *pointer, uint size, char *file, uint line) {
    uint i, j;
    for (i = 0; i < MEMORY_OVER_ALLOC; i++) {
        ((uint8 *)pointer)[size + i] = MEMORY_MAGIC_NUMBER;
    }

    for (i = 0; i < alloc_line_count; i++) {
        if (line == alloc_lines[i].line) {
            for (j = 0; file[j] != 0 && file[j] == alloc_lines[i].file[j]; j++);
            if (file[j] == alloc_lines[i].file[j]) {
                break;
            }
        }
    }
    if (i < alloc_line_count) {
        if (alloc_lines[i].alloc_alocated == alloc_lines[i].alloc_count) {
            alloc_lines[i].alloc_alocated += 1024;
            alloc_lines[i].allocs = realloc(alloc_lines[i].allocs, (sizeof *alloc_lines[i].allocs) * alloc_lines[i].alloc_alocated);
        }
        alloc_lines[i].allocs[alloc_lines[i].alloc_count].size = size;
        alloc_lines[i].allocs[alloc_lines[i].alloc_count++].buf = pointer;
        alloc_lines[i].size += size;
        alloc_lines[i].alocated++;
    } else {
        if (i < 1024) {
            alloc_lines[i].line = line;
            for (j = 0; j < 255 && file[j] != 0; j++) {
                alloc_lines[i].file[j] = file[j];
            }
            alloc_lines[i].file[j] = 0;
            alloc_lines[i].alloc_alocated = 256;
            alloc_lines[i].allocs = malloc((sizeof *alloc_lines[i].allocs) * alloc_lines[i].alloc_alocated);
            alloc_lines[i].allocs[0].size = size;
            alloc_lines[i].allocs[0].buf = pointer;
            alloc_lines[i].alloc_count = 1;
            alloc_lines[i].size = size;
            alloc_lines[i].freed = 0;
            alloc_lines[i].alocated++;
            alloc_line_count++;
        }
    }
}

void *debug_mem_malloc(uint size, char *file, uint line) {
    void *pointer;
    uint i;
    if (alloc_mutex != NULL) {
        alloc_mutex_lock(alloc_mutex);
    }
    pointer = malloc(size + MEMORY_OVER_ALLOC);

    if (pointer == NULL) {
        printf("MEM ERROR: Malloc returns NULL when trying to allocate %u bytes at line %u in file %s\n", size, line, file);
        if (alloc_mutex != NULL)
            alloc_mutex_unlock(alloc_mutex);
        debug_mem_print(0);
        exit(0);
    }
    for (i = 0; i < size + MEMORY_OVER_ALLOC; i++) {
         ((uint8 *)pointer)[i] = MEMORY_MAGIC_NUMBER + 1;
    }
    debug_mem_add(pointer, size, file, line);
    if (alloc_mutex != NULL) {
        alloc_mutex_unlock(alloc_mutex);
    }
    return pointer;
}

boolean debug_mem_remove(void *buf) {
    uint i, j, k;
    for (i = 0; i < alloc_line_count; i++) {
        for (j = 0; j < alloc_lines[i].alloc_count; j++) {
            if (alloc_lines[i].allocs[j].buf == buf) {
                for (k = 0; k < MEMORY_OVER_ALLOC; k++) {
                    if (((uint8 *)buf)[alloc_lines[i].allocs[j].size + k] != MEMORY_MAGIC_NUMBER) {
                        break;
                    }
                }
                if (k < MEMORY_OVER_ALLOC) {
                    printf("MEM ERROR: Overshoot at line %u in file %s\n", alloc_lines[i].line, alloc_lines[i].file);
                }
                alloc_lines[i].size -= alloc_lines[i].allocs[j].size;
                alloc_lines[i].allocs[j] = alloc_lines[i].allocs[--alloc_lines[i].alloc_count];
                alloc_lines[i].freed++;
                return TRUE;
            }
        }	
    }
    return FALSE;
}

void debug_mem_free(void *buf) {
    if (alloc_mutex != NULL) {
        alloc_mutex_lock(alloc_mutex);
    }
    if (!debug_mem_remove(buf)) {
        uint *X = NULL;
        X[0] = 0;
    }
    free(buf);
    if (alloc_mutex != NULL) {
        alloc_mutex_unlock(alloc_mutex);
    }
}

void *debug_mem_realloc(void *pointer, uint size, char *file, uint line) {
    uint i, j, k, move;
    void *pointer2;
    if (pointer == NULL) {
        return debug_mem_malloc( size, file, line);
    }
    
    if (alloc_mutex != NULL) {
        alloc_mutex_lock(alloc_mutex);
    }
    for (i = 0; i < alloc_line_count; i++) {
        for (j = 0; j < alloc_lines[i].alloc_count; j++) {
            if (alloc_lines[i].allocs[j].buf == pointer) {
                break;
            }
        }
        if (j < alloc_lines[i].alloc_count) {
            break;
        }
    }
    if (i == alloc_line_count) {
        printf("Mem debugger error. Trying to reallocate pointer %p in %s line %u. Pointer has never been allocated\n", pointer, file, line);
        for (i = 0; i < alloc_line_count; i++) {
            for (j = 0; j < alloc_lines[i].alloc_count; j++) {
                uint *buf;
                buf = alloc_lines[i].allocs[j].buf;
                for (k = 0; k < alloc_lines[i].allocs[j].size; k++) {
                    if (&buf[k] == pointer) {
                        printf("Trying to reallocate pointer %u bytes (out of %u) into allocation made in %s on line %u.\n", k, alloc_lines[i].allocs[j].size, alloc_lines[i].file, alloc_lines[i].line);
                    }
                }
            }
        }
        exit(0);
    }
    move = alloc_lines[i].allocs[j].size;
    
    if (move > size) {
        move = size;
    }

    pointer2 = malloc(size + MEMORY_OVER_ALLOC);
    if (pointer2 == NULL) {
        printf("MEM ERROR: Realloc returns NULL when trying to allocate %u bytes at line %u in file %s\n", size, line, file);
        if (alloc_mutex != NULL)
            alloc_mutex_unlock(alloc_mutex);
        debug_mem_print(0);
        exit(0);
    }
    for (i = 0; i < size + MEMORY_OVER_ALLOC; i++) {
         ((uint8 *)pointer2)[i] = MEMORY_MAGIC_NUMBER + 1;
    }
    memcpy(pointer2, pointer, move);

    debug_mem_add(pointer2, size, file, line);
    debug_mem_remove(pointer);
    free(pointer);

    if (alloc_mutex != NULL) {
        alloc_mutex_unlock(alloc_mutex);
    }
    return pointer2;
}

void debug_mem_print(uint min_allocs) {
    uint i;
    if (alloc_mutex != NULL) {
        alloc_mutex_lock(alloc_mutex);
    }
    printf("Memory report:\n----------------------------------------------\n");
    for (i = 0; i < alloc_line_count; i++) {
        if (min_allocs < alloc_lines[i].alocated) {
            printf("%s line: %u\n",alloc_lines[i].file, alloc_lines[i].line);
            printf(" - Bytes allocated: %u\n - Allocations: %u\n - Frees: %u\n\n", alloc_lines[i].size, alloc_lines[i].alocated, alloc_lines[i].freed);
        }
    }
    printf("----------------------------------------------\n");
    if (alloc_mutex != NULL) {
        alloc_mutex_unlock(alloc_mutex);
    }
}

uint32 debug_mem_consumption(void) {
    uint i, sum = 0;

    if (alloc_mutex != NULL) {
        alloc_mutex_lock(alloc_mutex);
    }

    for (i = 0; i < alloc_line_count; i++) {
        sum += alloc_lines[i].size;
    }

    if (alloc_mutex != NULL) {
        alloc_mutex_unlock(alloc_mutex);
    }

    return sum;
}

void debug_mem_reset(void) {
    uint i;

    if (alloc_mutex != NULL) {
        alloc_mutex_lock(alloc_mutex);
    }

    for (i = 0; i < alloc_line_count; i++) {
        free(alloc_lines[i].allocs);
    }
    alloc_line_count = 0;

    if (alloc_mutex != NULL) {
        alloc_mutex_unlock(alloc_mutex);
    }
}

void exit_crash(uint i) {
    uint *a = NULL;
    a[0] = 0;
}