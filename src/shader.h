#ifndef SHADER_H
#define SHADER_H

#include "kore.h"

const char *read_shader_from_file(const char *path);
GLuint load_shader(const char *vert_path, const char *frag_path);

#endif