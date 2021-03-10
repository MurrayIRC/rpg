#include "window.h"
#include "log.h"
#include <assert.h>

Window *window_create(const int32 w, const int32 h, const char* name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); OSX ONLY

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
    
    return window;
}

bool window_should_close(Window *window) {
    return glfwWindowShouldClose(window->glfw_window);
}

void window_destroy(Window *window) {
    free(window);
}