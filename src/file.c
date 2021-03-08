#include "file.h"
#include "log.h"

static void validate_file(file f) {
    if (f == NULL) {
        log_fatal("Trying to perform operations on an uninitialized file object.\n");
        exit(1);
    }
}

file file_open(const char* path, file_mode mode) {
    file f = malloc(sizeof(struct file));
    *f = (struct file) { .path = path };

    const char* file_mode_str = "";
    switch (mode) {
        case FILE_MODE_READ: file_mode_str = "r"; break;
        case FILE_MODE_READ_BINARY: file_mode_str = "rb"; break;
        case FILE_MODE_WRITE: file_mode_str = "w"; break;
        case FILE_MODE_WRITE_BINARY: file_mode_str = "wb"; break;
        case FILE_MODE_APPEND: file_mode_str = "a"; break;
        case FILE_MODE_APPEND_BINARY: file_mode_str = "ab"; break;
        case FILE_MODE_READ_WRITE: file_mode_str = "r+"; break;
        case FILE_MODE_READ_WRITE_BINARY: file_mode_str = "r+b"; break;
        default: log_fatal("Trying to open a file with an unknown FileMode %i.\n", mode); exit(1);
    }

    f->fp = fopen(path, file_mode_str);
    /* f->buffer = SDL_RWFromFile(path, fileMode); */
    if (f->fp == NULL) {
        log_fatal("Trying to open file %s that doesn't exist.\n", path);
        exit(1);
    }

    fseek(f->fp, 0, SEEK_END);
    f->size = ftell(f->fp);
    fseek(f->fp, 0, SEEK_SET);

    return f;
}

unsigned long file_get_size(file f) {
    validate_file(f);
    return f->size;
}

const char* file_get_path(file f) {
    validate_file(f);
    return f->path;
}

void file_read(file f, unsigned long offset, unsigned long size, void* data) {
    validate_file(f);

    if (offset > f->size) {
        log_fatal("Trying to read from the file, but offset + size (%l + %l) is outside the bounds of the file size %l.\n", offset, size, f->size);
        exit(1);
    }
    if (data == NULL) {
        log_fatal("Trying to copy data from the file into the pointer, but the supplied pointer is NULL.\n");
        exit(1);
    }

    fseek(f->fp, offset, SEEK_SET);
    fread(f->fp, size, 1, data); /* This is probably wrong. */
    fseek(f->fp, 0, SEEK_SET);

    /*SDL_RWseek(file->RW, offset, RW_SEEK_SET);*/
    /*SDL_RWread(file->RW, data, size, 1);*/
    /*SDL_RWseek(file->RW, 0, RW_SEEK_SET);*/
}

void file_write(file f, unsigned long offset, unsigned long size, void* data) {
    validate_file(f);
    if (offset > f->size) {
        log_fatal("Trying to write to the file, but offset + size (%l + %l) is outside the bounds of the file size %l.\n", offset, size, f->size);
        exit(1);
    }
    if (data == NULL) {
        log_fatal("Trying to copy data from the pointer into the file, but the supplied pointer is NULL.\n", f);
        exit(1);
    }

    fseek(f->fp, offset, SEEK_SET);
    fwrite(f->fp, size, 1, data); /* this is also probably wrong... */
    fseek(f->fp, 0, SEEK_SET);

    /*SDL_RWseek(f->RW, offset, RW_SEEK_SET);
    SDL_RWwrite(f->RW, data, size, 1);
    SDL_RWseek(f->RW, 0, RW_SEEK_SET);*/
}

boolean file_exists(const char* path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return FALSE;
    } else {
        fclose(f);
        return TRUE;
    }
}

void file_close(file f) {
    validate_file(f);
    fclose(f->fp);
    free(f);
}

/*
char* file_current_path(void) {
    //return SDL_GetBasePath();
    return "";
}
*/