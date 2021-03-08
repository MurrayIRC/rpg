#include "window.h"

#include <assert.h>

Window *window_create(const int32 w, const int32 h, const char* name) {
    Window *window;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = malloc(sizeof(*window));
    window->width = w;
    window->height = h;
    window->name = name;
    window->glfw_window = glfwCreateWindow(w, h, name, NULL, NULL);
    
    return window;
}

boolean window_should_close(Window *window) {
    return glfwWindowShouldClose(window->glfw_window);
}

boolean window_create_surface(VkInstance instance, GLFWwindow *glfw_window, VkSurfaceKHR* surface) {
    return glfwCreateWindowSurface(instance, glfw_window, NULL, surface);
}

void window_destroy(Window *window) {
    free(window);
}