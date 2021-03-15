#ifndef SHADER_H
#define SHADER_H

#include "core.h"

typedef struct {
    GLuint program_id;
} Shader;

/* geom_path can be NULL, because we might not always want to specify it, but
    vert_path & frag_path should always have values.*/
Shader *shader_create(const char *vert_path, const char *frag_path, const char *geom_path);
void shader_destroy(Shader *shader);
char *shader_read_from_file(const char *path);
void shader_use(Shader *shader);
void shader_check_shader_compile_errors(GLuint shader_id);
void shader_check_program_compile_errors(GLuint program_id);

// utility uniform functions
static inline void shader_shader_set__bool(Shader *shader, const char *name, const bool value) {
    glUniform1i(glGetUniformLocation(shader->program_id, name), (int)value);
}

static inline void shader_set_int(Shader *shader, const char *name, const int value) {
    glUniform1i(glGetUniformLocation(shader->program_id, name), value);
}

static inline void shader_set_float(Shader *shader, const char *name, const float value) {
    glUniform1f(glGetUniformLocation(shader->program_id, name), value);
}

static inline void shader_set_vec2(Shader *shader, const char *name, const float x, const float y) {
    glUniform2f(glGetUniformLocation(shader->program_id, name), x, y);
}

static inline void shader_set_vec3(Shader *shader, const char *name, const float x, const float y,
                                   const float z) {
    glUniform3f(glGetUniformLocation(shader->program_id, name), x, y, z);
}

static inline void shader_set_vec4(Shader *shader, const char *name, const float x, const float y,
                                   const float z, const float w) {
    glUniform4f(glGetUniformLocation(shader->program_id, name), x, y, z, w);
}

static inline void shader_set_mat2(Shader *shader, const char *name, const float *mat) {
    glUniformMatrix2fv(glGetUniformLocation(shader->program_id, name), 1, GL_FALSE, &mat[0]);
}

static inline void shader_set_mat3(Shader *shader, const char *name, const float *mat) {
    glUniformMatrix3fv(glGetUniformLocation(shader->program_id, name), 1, GL_FALSE, &mat[0]);
}

static inline void shader_set_mat4(Shader *shader, const char *name, const float *mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, name), 1, GL_FALSE, &mat[0]);
}

#endif