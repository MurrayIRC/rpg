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
void app_init_timer(void);

int app_get_current_monitor(void);

// core update functions --------------
void app_poll_input(void);
// ------------------------------------

// window functions -------------------
void app_window_toggle_fullscreen(void);
void app_window_maximize(void);
void app_window_minimize(void);
void app_window_set_title(const char *title);
// ------------------------------------

// frame & time -----------------------
void app_wait(float ms);
void app_set_fps_target(int fps);
float app_get_fps(void);
// ------------------------------------

#endif
