/* date = February 28th 2021 6:52 pm */

#ifndef WINDOW_H
#define WINDOW_H

#include "kore.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

typedef struct {
    int32 width;
    int32 height;
    
    const char* name;
    GLFWwindow* glfw_window;
} Window;

Window *window_create(const int32 w, const int32 h, const char* name);
boolean window_should_close(Window *window);
boolean window_create_surface(VkInstance instance, GLFWwindow *glfw_window, VkSurfaceKHR* surface);
void window_destroy(Window *window);

#endif
