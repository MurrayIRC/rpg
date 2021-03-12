#ifndef RENDER_H
#define RENDER_H

#include "kore.h"
#include "shader.h"
#include "window.h"

typedef struct {
    GLuint vertex_array_id;
    GLuint vertex_buffer;
    GLuint color_buffer;
    Shader *simple_shader;

    GLuint mvp_uniform_id;
    float mvp[16];
} Renderer;

Renderer *renderer_create(Window *win);
void renderer_draw_frame(Renderer *renderer);
void renderer_destroy(Renderer *renderer);

#endif