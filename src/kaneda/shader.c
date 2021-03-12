#include "shader.h"
#include "file.h"
#include "log.h"

Shader *shader_create(const char *vert_path, const char *frag_path) {
    Shader *shader;
    shader = malloc(sizeof(*shader));

    // Create the shaders
    GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    shader->vert_code = (const char *)shader_read_from_file(vert_path);
    if (shader->vert_code == NULL) {
        log_error("Unable to read vertex shader code. Check the file path.");
    }

    shader->frag_code = (const char *)shader_read_from_file(frag_path);
    if (shader->frag_code == NULL) {
        log_error("Unable to read fragment shader code. Check the file path.");
    }

    GLint result = GL_FALSE;
    int info_log_length;

    // Compile Vertex Shader
    log_info("Compiling shader: %s\n", vert_path);
    glShaderSource(vert_shader_id, 1, &shader->vert_code, NULL);
    glCompileShader(vert_shader_id);

    // Check Vertex Shader
    glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vert_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char *vert_shader_error_msg = malloc(sizeof(const char *) * (info_log_length + 1));
        glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &vert_shader_error_msg[0]);
        log_error("%s\n", &vert_shader_error_msg[0]);
        free(vert_shader_error_msg);
    }

    // Compile Fragment Shader
    log_info("Compiling shader: %s\n", frag_path);
    glShaderSource(frag_shader_id, 1, &shader->frag_code, NULL);
    glCompileShader(frag_shader_id);

    // Check Fragment Shader
    glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(frag_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char *frag_shader_error_msg = malloc(sizeof(const char *) * (info_log_length + 1));
        glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &frag_shader_error_msg[0]);
        log_error("%s\n", &frag_shader_error_msg[0]);
        free(frag_shader_error_msg);
    }

    // Link the program
    log_info("Linking program...\n");
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vert_shader_id);
    glAttachShader(program_id, frag_shader_id);
    glLinkProgram(program_id);

    // Check the program
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char *program_error_msg = malloc(sizeof(const char *) * (info_log_length + 1));
        glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &program_error_msg[0]);
        log_error("%s\n", &program_error_msg[0]);
        free(program_error_msg);
    }

    glDetachShader(program_id, vert_shader_id);
    glDetachShader(program_id, frag_shader_id);

    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);

    shader->program_id = program_id;

    return shader;
}

char *shader_read_from_file(const char *path) {
    file f = file_open(path, FILE_MODE_READ);
    char *code;
    code = malloc(sizeof(*code) * f->size);
    file_read(f, 0, f->size, code);
    code[f->size] = '\0';
    file_close(f);
    // printf("\nCODE--------\n%s\n------------", code);

    return code;
}

void shader_use(Shader *shader) {
    glUseProgram(shader->program_id);
}

void shader_destroy(Shader *shader) {
    glDeleteProgram(shader->program_id);
    free(shader->vert_code);
    free(shader->frag_code);
    free(shader);
}