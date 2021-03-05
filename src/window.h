/* date = February 28th 2021 6:52 pm */

#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdio.h>

typedef struct window {
    int width;
    int height;
    
    const char* name;
    GLFWwindow* glfw_window;
} window;

window window_init(int w, int h, const char* name);
bool window_should_close(window window);
bool window_create_surface(GLFWwindow* window, VkInstance instance, VkSurfaceKHR* surface);

#endif
