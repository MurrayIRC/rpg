#include "renderer.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer *renderer_create(void) {
    Renderer *renderer;
    renderer = malloc(sizeof(*renderer));

    // renderer->width = win->width;
    // renderer->height = win->height;

    renderer->camera = camera_create(math_vec3(0.0f, 0.0f, 3.0f), math_vec3(0.0f, 1.0f, 0.0f),
                                     CAMERA_DEFAULT_YAW, CAMERA_DEFAULT_PITCH);
    renderer->last_x = renderer->width / 2.0f;
    renderer->last_y = renderer->height / 2.0f;
    renderer->first_mouse = true;

    glEnable(GL_DEPTH_TEST);

    renderer->simple_shader = shader_create("shaders/simple.vert", "shaders/simple.frag", NULL);
    if (renderer->simple_shader == NULL) {
        log_fatal("Couldn't create the shader.\n");
        return NULL;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
                        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
                        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    vec3 cubes[] = {math_vec3(0.0f, 0.0f, 0.0f),    math_vec3(2.0f, 5.0f, -15.0f),
                    math_vec3(-1.5f, -2.2f, -2.5f), math_vec3(-3.8f, -2.0f, -12.3f),
                    math_vec3(2.4f, -0.4f, -3.5f),  math_vec3(-1.7f, 3.0f, -7.5f),
                    math_vec3(1.3f, -2.0f, -2.5f),  math_vec3(1.5f, 2.0f, -2.5f),
                    math_vec3(1.5f, 0.2f, -1.5f),   math_vec3(-1.3f, 1.0f, -1.5f)};
    renderer->cube_positions = &cubes;

    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    // glGenBuffers(1, &renderer->ebo);

    glBindVertexArray(renderer->vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    float current_frame = (float)glfwGetTime();
    renderer->delta_time = current_frame - renderer->last_frame;
    renderer->last_frame = current_frame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->tex_container);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderer->tex_face);

    shader_use(renderer->simple_shader);

    // pass projection to shader
    mat4 projection = math_mat4_identity();
    projection = math_perspective(renderer->camera->zoom,
                                  (float)renderer->width / (float)renderer->height, 0.1f, 100.0f);
    // projection = math_orthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    shader_set_mat4(renderer->simple_shader, "projection", &projection.elements);

    // camera/view transformation
    mat4 view = camera_get_view_matrix(renderer->camera);
    shader_set_mat4(renderer->simple_shader, "view", &view.elements);

    glBindVertexArray(renderer->vao);
    for (uint32 i = 0; i < 10; i++) {
        mat4 model = math_mat4_identity();
        model = math_translate(renderer->cube_positions[i]);
        float angle = 20.0f * i;
        mat4 rotation = math_rotate(angle, math_vec3(1.0f, 0.3f, 0.5f));
        model = math_mat4_multiply(model, rotation);

        shader_set_mat4(renderer->simple_shader, "model", &model.elements);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void renderer_destroy(Renderer *renderer) {
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteBuffers(1, &renderer->vbo);
    // glDeleteBuffers(1, &renderer->ebo);

    shader_destroy(renderer->simple_shader);
    camera_destroy(renderer->camera);
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
    // math_mat4_multiply(&p_v, &projection, &view);
    // math_mat4_multiply(&renderer->mvp, &p_v, &model);
    // ------------------------

*/