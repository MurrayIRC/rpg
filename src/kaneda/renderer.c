#include "renderer.h"
#include "log.h"

Renderer *renderer_create(Window *win) {
    Renderer *renderer;
    renderer = malloc(sizeof(*renderer));

    renderer->simple_shader = shader_create("shaders/simple.vert", "shaders/simple.frag");

    float vertices[18] = {
        // positions        // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
    };

    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glBindVertexArray(renderer->vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return renderer;
}

void renderer_draw_frame(Renderer *renderer) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_use(renderer->simple_shader);

    glBindVertexArray(renderer->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void renderer_destroy(Renderer *renderer) {
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteBuffers(1, &renderer->vbo);

    shader_destroy(renderer->simple_shader);
    free(renderer);
}

/*

// // Get a handle for our "MVP" uniform
    // renderer->mvp_uniform_id = glGetUniformLocation(renderer->simple_shader->program_id, "mvp");

    // float projection[16];
    // math_perspective(&projection, 45.0f, (float)win->width / (float)win->width, 0.1f, 100.0f);

    // // Or, for an ortho camera :
    // // math_orthographic(&projection, 10.0f, 0.0f, 100.0f);

    // // Camera matrix
    // float view[16];
    // float eye[3] = {4.0f, 3.0f, -3.0f};   // Camera is at (4,3,-3), in World Space
    // float center[3] = {0.0f, 0.0f, 0.0f}; // and looks at the origin
    // float up[3] = {0.0f, 1.0f, 0.0f};     // Head is up (set to 0,-1,0 for upside-down)
    // math_lookat(&view, &eye, &center, &up);

    // // Model matrix : an identity matrix (model will be at the origin)
    // float model[16];
    // math_matrix_identity(&model);

    // // Our ModelViewProjection : multiplication of our 3 matrices
    // // REMEMBER, matrix multiplication is the other way around
    // float p_v[16];
    // math_matrix_multiply(&p_v, &projection, &view);
    // math_matrix_multiply(&renderer->mvp, &p_v, &model);
    // ------------------------

*/