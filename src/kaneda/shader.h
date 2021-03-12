#ifndef SHADER_H
#define SHADER_H

#include "kore.h"

typedef struct {
    const char *vert_code;
    const char *frag_code;
    GLuint program_id;
} Shader;

Shader *shader_create(const char *vert_path, const char *frag_path);
char *shader_read_from_file(const char *path);
void shader_use(Shader *shader);
void shader_destroy(Shader *shader);

#endif