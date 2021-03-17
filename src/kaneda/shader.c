#include "shader.h"
#include "file.h"
#include "log.h"

bool shader_create(Shader *shader, const char *vert_path, const char *frag_path,
                   const char *geom_path) {
    if (vert_path == NULL || frag_path == NULL) {
        log_fatal("Vertex or Fragment shader paths not specified in shader creation.\n");
        return false;
    }

    // Read the Vertex Shader code from the file
    const char *vert_code = (const char *)shader_read_from_file(vert_path);
    if (vert_code == NULL) {
        log_error("Unable to read vertex shader code. Check the file path.");
        return false;
    }

    const char *frag_code = (const char *)shader_read_from_file(frag_path);
    if (frag_code == NULL) {
        log_error("Unable to read fragment shader code. Check the file path.");
        return false;
    }

    // Create the shaders
    GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile Vertex Shader
    log_info("Compiling shader: %s\n", vert_path);
    glShaderSource(vert_shader_id, 1, &vert_code, NULL);
    glCompileShader(vert_shader_id);
    shader_check_shader_compile_errors(vert_shader_id);

    // Compile Fragment Shader
    log_info("Compiling shader: %s\n", frag_path);
    glShaderSource(frag_shader_id, 1, &frag_code, NULL);
    glCompileShader(frag_shader_id);
    shader_check_shader_compile_errors(frag_shader_id);

    GLuint geom_shader_id;
    if (geom_path != NULL) {
        const char *geom_code = (const char *)shader_read_from_file(frag_path);
        if (geom_code == NULL) {
            log_error("Unable to read geometry shader code. Check the file path.");
            return false;
        }
        geom_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
        log_info("Compiling shader: %s\n", geom_path);
        glShaderSource(geom_shader_id, 1, &geom_code, NULL);
        glCompileShader(geom_shader_id);
        shader_check_shader_compile_errors(geom_shader_id);
    }

    // Link the program
    log_info("Linking program...\n");
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vert_shader_id);
    glAttachShader(program_id, frag_shader_id);
    if (geom_path != NULL) {
        glAttachShader(program_id, geom_shader_id);
    }
    glLinkProgram(program_id);
    shader_check_program_compile_errors(program_id);

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);
    if (geom_path != NULL) {
        glDeleteShader(geom_shader_id);
    }

    shader->program_id = program_id;
    return true;
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

void shader_check_shader_compile_errors(GLuint shader_id) {
    GLint result = GL_FALSE;
    int info_log_length;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char *error_msg = malloc(sizeof(const char *) * (info_log_length + 1));
        glGetShaderInfoLog(shader_id, info_log_length, NULL, &error_msg[0]);
        log_error("%s\n", &error_msg[0]);
        free(error_msg);
    }
}

void shader_check_program_compile_errors(GLuint program_id) {
    GLint result = GL_FALSE;
    int info_log_length;

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char *error_msg = malloc(sizeof(const char *) * (info_log_length + 1));
        glGetShaderInfoLog(program_id, info_log_length, NULL, &error_msg[0]);
        log_error("%s\n", &error_msg[0]);
        free(error_msg);
    }
}

void shader_destroy(Shader *shader) {
    glDeleteProgram(shader->program_id);
}