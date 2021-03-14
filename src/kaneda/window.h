/* date = February 28th 2021 6:52 pm */

#ifndef WINDOW_H
#define WINDOW_H

#include "kore.h"
#include <stdio.h>

typedef struct {
    int32 width;
    int32 height;

    const char *name;
    GLFWwindow *glfw_window;
} Window;

Window *window_create(const int32 w, const int32 h, const char *name);
void window_framebuffer_size_callback(GLFWwindow *window, int width, int height);
void window_destroy(Window *window);

#endif
