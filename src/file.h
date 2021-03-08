#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdbool.h>

typedef enum file_mode {
    /* Reads text from a file. 
    The file must exist */
    FILE_MODE_READ,
    /* Reads binary data from a file.
    The file must exist */
    FILE_MODE_READ_BINARY,
    /* Writes text to a file.
    If the file exists then it's erased.
    If the file doesn't exist then it's created */
    FILE_MODE_WRITE,
    /* Writes binary data to a file.
    If the file exists then it's erased.
    If the file doesn't exist then it's created */
    FILE_MODE_WRITE_BINARY,
    /*Appends text to the end of a file.
    The file is created if it doesn't exist */
    FILE_MODE_APPEND,
    /* Appends binary data to the end of a file.
    The file is created if it doesn't exist */
    FILE_MODE_APPEND_BINARY,
    /* Reads and writes text to a file.
    The file must exist */
    FILE_MODE_READ_WRITE,
    /* Reads and writes binary data to a file.
    The file must exist */
    FILE_MODE_READ_WRITE_BINARY,
} file_mode;

typedef struct file {
    const char *path;
    unsigned long size;
    FILE *fp;
} *file;

/*  
    Opens a file from the given path for read/write operations
        \param path The file path
        \param mode The read/write mode to open the file with
        \return The file object 
*/
file file_open(const char *path, file_mode mode);

/*
    Gets the size from a file object
        \param f The file object
        \return The file size
*/
unsigned long file_get_size(file f);

/*
    Gets the path from a file object
        \param f The file object
        \return The file path
*/
const char* file_get_path(file f);

/*
    Reads data from a file
        \param f The file object to read from
        \param offset The offset in bytes to read from
        \param size The size in bytes to read
        \param data A preallocated pointer that the memory will be copied to
*/
void file_read(file f, unsigned long offset, unsigned long size, void *data);

/*
    Writes data to a file
        \param f The file object to write to
        \param offset The offset in bytes to read at
        \param size The size in bytes to write
        \param data A pointer to the data to write
*/
void file_write(file f, unsigned long offset, unsigned long size, void *data);

/*
    Checks if a file exists
        \param path The path to check
        \return Either true or false if it exists
*/
bool file_exists(const char *path);

/*
    Closes the file object
        \param f The file object to close
*/
void file_close(file f);

/*
    Gets the current path that the process is running from.
        \return The path that it's running on
*/
char* file_current_path(void);

#endif