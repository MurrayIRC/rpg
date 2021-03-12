#include "renderer.h"

Renderer *renderer_create(Window *win) {
    Renderer *renderer;
    renderer = malloc(sizeof(*renderer));

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &renderer->vertex_array_id);
    glBindVertexArray(renderer->vertex_array_id);

    // SHADER SHIT ------------
    renderer->simple_shader = shader_create("shaders/simple.vert", "shaders/simple.frag");

    // Get a handle for our "MVP" uniform
    renderer->mvp_uniform_id = glGetUniformLocation(renderer->simple_shader->program_id, "mvp");

    float projection[16];
    math_perspective(&projection, math_deg2rad(45.0f), (float)win->width / (float)win->width, 0.1f,
                     100.0f);

    // Or, for an ortho camera :
    // math_orthographic(&projection, 10.0f, 0.0f, 100.0f);

    // Camera matrix
    float view[16];
    math_matrix_identity(&view);
    float eye = {4.0f, 3.0f, -3.0f};   // Camera is at (4,3,-3), in World Space
    float center = {0.0f, 0.0f, 0.0f}; // and looks at the origin
    float up = {0.0f, 1.0f, 0.0f};     // Head is up (set to 0,-1,0 for upside-down)
    math_lookat(&view, &eye, &center, &up);

    // Model matrix : an identity matrix (model will be at the origin)
    float model[16];
    math_matrix_identity(&model);

    // Our ModelViewProjection : multiplication of our 3 matrices
    // REMEMBER, matrix multiplication is the other way around
    float p_v[16];
    math_matrix_multiply(&p_v, &projection, &view);
    math_matrix_multiply(&renderer->mvp, &p_v, &model);
    // ------------------------

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
        1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f};

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        0.583f, 0.771f, 0.014f, 0.609f, 0.115f, 0.436f, 0.327f, 0.483f, 0.844f, 0.822f, 0.569f,
        0.201f, 0.435f, 0.602f, 0.223f, 0.310f, 0.747f, 0.185f, 0.597f, 0.770f, 0.761f, 0.559f,
        0.436f, 0.730f, 0.359f, 0.583f, 0.152f, 0.483f, 0.596f, 0.789f, 0.559f, 0.861f, 0.639f,
        0.195f, 0.548f, 0.859f, 0.014f, 0.184f, 0.576f, 0.771f, 0.328f, 0.970f, 0.406f, 0.615f,
        0.116f, 0.676f, 0.977f, 0.133f, 0.971f, 0.572f, 0.833f, 0.140f, 0.616f, 0.489f, 0.997f,
        0.513f, 0.064f, 0.945f, 0.719f, 0.592f, 0.543f, 0.021f, 0.978f, 0.279f, 0.317f, 0.505f,
        0.167f, 0.620f, 0.077f, 0.347f, 0.857f, 0.137f, 0.055f, 0.953f, 0.042f, 0.714f, 0.505f,
        0.345f, 0.783f, 0.290f, 0.734f, 0.722f, 0.645f, 0.174f, 0.302f, 0.455f, 0.848f, 0.225f,
        0.587f, 0.040f, 0.517f, 0.713f, 0.338f, 0.053f, 0.959f, 0.120f, 0.393f, 0.621f, 0.362f,
        0.673f, 0.211f, 0.457f, 0.820f, 0.883f, 0.371f, 0.982f, 0.099f, 0.879f};

    glGenBuffers(1, &renderer->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &renderer->color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    // ------------------------

    return renderer;
}

void renderer_draw_frame(Renderer *renderer) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderer->simple_shader->program_id);

    // Send our transformation to the currently bound shader, in the "MVP"
    // uniform This is done in the main loop since each model will have a
    // different MVP matrix (At least for the M part)
    glUniformMatrix4fv(renderer->mvp_uniform_id, 1, GL_FALSE, &renderer->mvp[0]);

    // 1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
    glVertexAttribPointer(0,        // attribute 0. Must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void *)0 // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->color_buffer);
    glVertexAttribPointer(1,        // attribute. No particular reason for 1, but
                                    // must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void *)0 // array buffer offset
    );

    // Draw the triangle! Starting from vertex 0; 3 vertices total makes 1 triangle
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void renderer_destroy(Renderer *renderer) {
    glDeleteBuffers(1, &renderer->vertex_buffer);
    glDeleteBuffers(1, &renderer->color_buffer);

    shader_destroy(renderer->simple_shader);
    glDeleteVertexArrays(1, &renderer->vertex_array_id);
    free(renderer);
}