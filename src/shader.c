#include "shader.h"
#include "file.h"
#include "log.h"

const char *read_shader_from_file(const char *path) {
    file f = file_open(path, FILE_MODE_READ);
    const char *code = malloc(sizeof(const char *) * f->size);
    file_read(f, 0, f->size, &code);

    return code;
}

GLuint load_shader(const char *vert_path, const char *frag_path) {
    // Create the shaders
    GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    const char* vert_shader_code = read_shader_from_file(vert_path);
    if (vert_shader_code == NULL) {
        log_error("Unable to read vertex shader code. Check the file path.");
    }

    const char* frag_shader_code = read_shader_from_file(frag_path);
    if (frag_shader_code == NULL) {
        log_error("Unable to read fragment shader code. Check the file path.");
    }

    GLint result = GL_FALSE;
    int info_log_length;

    // Compile Vertex Shader
    log_info("Compiling shader: %s\n", vert_path);
    glShaderSource(vert_shader_id, 1, &vert_shader_code, NULL);
    glCompileShader(vert_shader_id);

    // Check Vertex Shader
    glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vert_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char* vert_shader_error_msg = malloc(sizeof(const char*) * (info_log_length + 1));
        glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &vert_shader_error_msg[0]);
        log_error("%s\n", &vert_shader_error_msg[0]);
        free(vert_shader_error_msg);
    }

    // Compile Fragment Shader
    log_info("Compiling shader: %s\n", frag_path);
    glShaderSource(frag_shader_id, 1, &frag_shader_code , NULL);
    glCompileShader(frag_shader_id);

    // Check Fragment Shader
    glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(frag_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char* frag_shader_error_msg = malloc(sizeof(const char*) * (info_log_length + 1));
        glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &frag_shader_error_msg[0]);
        log_error("%s\n", &frag_shader_error_msg[0]);
        free(frag_shader_error_msg);
    }

    // Link the program
    printf("Linking program...\n");
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vert_shader_id);
    glAttachShader(program_id, frag_shader_id);
    glLinkProgram(program_id);

    // Check the program
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        const char* program_error_msg = malloc(sizeof(const char*) * (info_log_length + 1));
        glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &program_error_msg[0]);
        log_error("%s\n", &program_error_msg[0]);
        free(program_error_msg);
    }
    
    glDetachShader(program_id, vert_shader_id);
    glDetachShader(program_id, frag_shader_id);
    
    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);

    return program_id;
}
