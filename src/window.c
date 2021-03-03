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

void window_create_surface(GLFWwindow* window, VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, window, NULL, surface) != VK_SUCCESS) {
        printf("[ERROR]: Failed to create window surface.");
    }
}