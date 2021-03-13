#ifndef RENDER_H
#define RENDER_H

#include "kore.h"
#include "shader.h"
#include "window.h"

typedef struct {
    uint32 width;
    uint32 height;

    Shader *simple_shader;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    unsigned int tex_container;
    unsigned int tex_face;

    float *cube_positions;
} Renderer;

Renderer *renderer_create(Window *win);
void renderer_draw_frame(Renderer *renderer);
void renderer_destroy(Renderer *renderer);

#endif