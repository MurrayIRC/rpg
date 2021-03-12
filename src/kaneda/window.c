#include "window.h"
#include "log.h"
#include <assert.h>

Window *window_create(const int32 w, const int32 h, const char *name) {
    if (!glfwInit()) {
        log_fatal("Failed to initialize GLFW\n");
        return NULL;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
#ifdef PLATFORM_APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Window *window;
    window = malloc(sizeof(*window));
    window->width = w;
    window->height = h;
    window->name = name;
    window->glfw_window = glfwCreateWindow(w, h, name, NULL, NULL);
    if (window->glfw_window == NULL) {
        log_fatal("Failed to create GLFW window.");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window->glfw_window);
    glfwSetFramebufferSizeCallback(window->glfw_window, window_framebuffer_size_callback);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        log_fatal("Failed to initialize OpenGL context.\n");
        return NULL;
    }
    log_info("Loaded OpenGL %d.%d.\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    return window;
}

void window_process_input(Window *window) {
    if (glfwGetKey(window->glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window->glfw_window, true);
    }
}

void window_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void window_destroy(Window *window) {
    glfwTerminate();
    free(window);
}