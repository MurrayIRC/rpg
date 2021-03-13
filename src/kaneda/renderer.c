#include "renderer.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer *renderer_create(Window *win) {
    Renderer *renderer;
    renderer = malloc(sizeof(*renderer));

    renderer->simple_shader = shader_create("shaders/simple.vert", "shaders/simple.frag", NULL);
    if (renderer->simple_shader == NULL) {
        log_fatal("Couldn't create the shader.\n");
        return NULL;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions        // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glGenBuffers(1, &renderer->ebo);

    glBindVertexArray(renderer->vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    // texture 1
    // ---------
    glGenTextures(1, &renderer->tex_container);
    glBindTexture(GL_TEXTURE_2D, renderer->tex_container);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./assets/container.jpg", &width, &height, &num_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        log_fatal("Failed to load texture.\n");
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &renderer->tex_face);
    glBindTexture(GL_TEXTURE_2D, renderer->tex_face);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./assets/face.png", &width, &height, &num_channels, 0);
    if (data) {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to
        // tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        log_fatal("Failed to load texture.\n");
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader_use(renderer->simple_shader);
    shader_set_int(renderer->simple_shader, "texture1", 0);
    shader_set_int(renderer->simple_shader, "texture2", 1);

    return renderer;
}

void renderer_draw_frame(Renderer *renderer) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->tex_container);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderer->tex_face);

    // create transformations
    float transform[16];
    math_matrix_identity(&transform);
    math_translate(&transform, 0.5f, -0.5f, 0.0f);
    math_rotate(&transform, (float)glfwGetTime(), 0.0f, 0.0f, 1.0f);

    // get matrix's uniform location and set matrix
    shader_use(renderer->simple_shader);
    shader_set_mat4(renderer->simple_shader, "transform", &transform);

    glBindVertexArray(renderer->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderer_destroy(Renderer *renderer) {
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteBuffers(1, &renderer->vbo);
    glDeleteBuffers(1, &renderer->ebo);

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