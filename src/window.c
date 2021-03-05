#include "window.h"

#include <assert.h>

window window_init(int w, int h, const char* name) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window window = {w, h, name, glfwCreateWindow(w, h, name, NULL, NULL)};
    return window;
}

bool window_should_close(window window) {
    return glfwWindowShouldClose(window.glfw_window);
}

bool window_create_surface(GLFWwindow* window, VkInstance instance, VkSurfaceKHR* surface) {
    return glfwCreateWindowSurface(instance, window, NULL, surface);
}