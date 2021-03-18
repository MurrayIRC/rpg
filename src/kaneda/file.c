#include "file.h"
#include "log.h"

#include <assert.h>

static void validate_file(file f) {
    if (f == NULL) {
        log_fatal("Trying to perform operations on an uninitialized file object.\n");
        exit(1);
    }
}

file file_open(const char *path, file_mode mode) {
    file f = malloc(sizeof(struct file));
    *f = (struct file){.path = path};

    const char *file_mode_str = "";
    switch (mode) {
    case FILE_MODE_READ:
        file_mode_str = "r";
        break;
    case FILE_MODE_READ_BINARY:
        file_mode_str = "rb";
        break;
    case FILE_MODE_WRITE:
        file_mode_str = "w";
        break;
    case FILE_MODE_WRITE_BINARY:
        file_mode_str = "wb";
        break;
    case FILE_MODE_APPEND:
        file_mode_str = "a";
        break;
    case FILE_MODE_APPEND_BINARY:
        file_mode_str = "ab";
        break;
    case FILE_MODE_READ_WRITE:
        file_mode_str = "r+";
        break;
    case FILE_MODE_READ_WRITE_BINARY:
        file_mode_str = "r+b";
        break;
    default:
        log_fatal("Trying to open a file with an unknown FileMode %i.\n", mode);
        exit(1);
    }

    f->fp = fopen(path, file_mode_str);
    if (f->fp == NULL) {
        log_fatal("Trying to open file %s that doesn't exist.\n", path);
        exit(1);
    }

#ifdef PLATFORM_WINDOWS
    HANDLE h_file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h_file == INVALID_HANDLE_VALUE) {
        return -1; // error condition, could call GetLastError to find out more
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(h_file, &size)) {
        CloseHandle(h_file);
        return -1; // error condition, could call GetLastError to find out more
    }

    CloseHandle(h_file);
    assert(size.QuadPart <= 0xFFFFFFFF);
    f->size = (uint32)size.QuadPart;
#else
    struct stat st;
    stat(path, &st);
    f->size = (uint32)st.st_size;
#endif

    // fseek(f->fp, 0, SEEK_END);
    // f->size = ftell(f->fp);
    // fseek(f->fp, 0, SEEK_SET);

    return f;
}

uint32 file_get_size(file f) {
    validate_file(f);
    return f->size;
}

const char *file_get_path(file f) {
    validate_file(f);
    return f->path;
}

void file_read(file f, uint32 offset, uint32 size, void *data) {
    validate_file(f);

    if (offset > f->size) {
        log_fatal("Trying to read from the file, but offset + size (%l + %l) is outside the bounds "
                  "of the file size %l.\n",
                  offset, size, f->size);
        exit(1);
    }
    if (data == NULL) {
        log_fatal("Trying to copy data from the file into the pointer, but the supplied pointer is "
                  "NULL.\n");
        exit(1);
    }

    fseek(f->fp, offset, SEEK_SET);
    fread(data, size, 1, f->fp); /* This is probably wrong. */
    fseek(f->fp, 0, SEEK_SET);
}

void file_write(file f, uint32 offset, uint32 size, void *data) {
    validate_file(f);
    if (offset > f->size) {
        log_fatal("Trying to write to the file, but offset + size (%l + %l) is outside the bounds "
                  "of the file size %l.\n",
                  offset, size, f->size);
        exit(1);
    }
    if (data == NULL) {
        log_fatal("Trying to copy data from the pointer into the file, but the supplied pointer is "
                  "NULL.\n",
                  f);
        exit(1);
    }

    fseek(f->fp, offset, SEEK_SET);
    fwrite(data, size, 1, f->fp); /* this is also probably wrong... */
    fseek(f->fp, 0, SEEK_SET);
}

bool file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return false;
    } else {
        fclose(f);
        return true;
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