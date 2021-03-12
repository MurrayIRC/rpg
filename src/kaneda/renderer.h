#ifndef RENDER_H
#define RENDER_H

#include "kore.h"
#include "shader.h"
#include "window.h"

typedef struct {
    Shader *simple_shader;
    GLuint vao;
    GLuint vbo;

    GLuint mvp_uniform_id;
    float mvp[16];
} Renderer;

Renderer *renderer_create(Window *win);
void renderer_draw_frame(Renderer *renderer);
void renderer_destroy(Renderer *renderer);

#endif