#ifndef RENDER_H
#define RENDER_H

#include "core.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Shader shader;
Camera camera;

static inline bool renderer_init(const uint32 w, const uint32 h) {
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        log_fatal("Failed to initialize OpenGL context.\n");
        return false;
    }
    log_info("Loaded OpenGL %d.%d.\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    glViewport(CORE.Window.render_offset.x / 2, CORE.Window.render_offset.y / 2,
               CORE.Window.render_size.width - CORE.Window.render_offset.x,
               CORE.Window.render_size.height - CORE.Window.render_offset.y);

    Camera *camera = camera_create(math_vec3(0.0f, 0.0f, 3.0f), math_vec3(0.0f, 1.0f, 0.0f),
                                   CAMERA_DEFAULT_YAW, CAMERA_DEFAULT_PITCH);

    glEnable(GL_DEPTH_TEST);

    if (!shader_create(&shader, "shaders/simple.vert", "shaders/simple.frag", NULL)) {
        log_fatal("Couldn't create the shader.\n");
        return false;
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

    CORE.Renderer.cube_pos = math_vec3(0.0f, 0.0f, 0.0f);

    glGenVertexArrays(1, &CORE.Renderer.vao);
    glGenBuffers(1, &CORE.Renderer.vbo);

    glBindVertexArray(CORE.Renderer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, CORE.Renderer.vbo);
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
    glGenTextures(1, &CORE.Renderer.tex_container);
    glBindTexture(GL_TEXTURE_2D, CORE.Renderer.tex_container);
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
    glGenTextures(1, &CORE.Renderer.tex_face);
    glBindTexture(GL_TEXTURE_2D, CORE.Renderer.tex_face);
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
    shader_use(&shader);
    shader_set_int(&shader, "texture1", 0);
    shader_set_int(&shader, "texture2", 1);

    return true;
}

static inline void renderer_draw_frame() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, CORE.Renderer.tex_container);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, CORE.Renderer.tex_face);

    shader_use(&shader);

    // pass projection to shader
    mat4 projection = math_mat4_identity();
    projection = math_perspective(
        camera.zoom, (float)CORE.Window.render_size.width / (float)CORE.Window.render_size.height,
        0.1f, 100.0f);
    // projection = math_orthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    shader_set_mat4(&shader, "projection", &projection.elements);

    // camera/view transformation
    mat4 view = camera_get_view_matrix(&camera);
    shader_set_mat4(&shader, "view", &view.elements);

    glBindVertexArray(CORE.Renderer.vao);

    mat4 model = math_mat4_identity();
    model = math_translate(CORE.Renderer.cube_pos);
    float angle = 20.0f;
    mat4 rotation = math_rotate(angle, math_vec3(1.0f, 0.3f, 0.5f));
    model = math_mat4_multiply(model, rotation);

    shader_set_mat4(&shader, "model", &model.elements);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

static inline void renderer_cleanup() {
    glDeleteVertexArrays(1, &CORE.Renderer.vao);
    glDeleteBuffers(1, &CORE.Renderer.vbo);
    // glDeleteBuffers(1, &renderer->ebo);

    shader_destroy(&shader);
}

#endif