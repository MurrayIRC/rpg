#include "window.h"
#include "log.h"
#include <assert.h>

Window *window_create(const int32 w, const int32 h, const char *name) {
    glewExperimental = true;

    if (!glfwInit()) {
        log_fatal("Failed to initialize GLFW\n");
        return NULL;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
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

    if (glewInit() != GLEW_OK) {
        log_fatal("Failed to initialize GLEW\n");
        return NULL;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

bool window_should_close(Window *window) {
    return glfwGetKey(window->glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
           glfwWindowShouldClose(window->glfw_window);
}

void window_destroy(Window *window) {
    free(window);
}