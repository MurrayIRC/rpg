#ifndef RENDER_H
#define RENDER_H

#include "core.h"
#include "shader.h"
#include "camera.h"
#include "math.h"

typedef struct {
    uint32 width;
    uint32 height;

    Shader *simple_shader;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    unsigned int tex_container;
    unsigned int tex_face;

    vec3 *cube_positions;

    Camera *camera;
    float last_x;
    float last_y;
    bool first_mouse;

    float delta_time;
    float last_frame;
} Renderer;

Renderer *renderer_create(void);
void renderer_draw_frame(Renderer *renderer);
void renderer_destroy(Renderer *renderer);

#endif