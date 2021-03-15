/* date = February 28th 2021 6:26 pm */

#ifndef APP_H
#define APP_H

#include "core.h"

static const int32 WINDOW_WIDTH = 800;
static const int32 WINDOW_HEIGHT = 600;

bool app_run(void);
void app_shutdown(void);

bool app_init_glfw(void);
bool app_init_window(const uint32 w, const uint32 h, const char *name);
bool app_init_renderer(const uint32 w, const uint32 h);
void window_framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif
