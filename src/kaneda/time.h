#ifndef TIME_H
#define TIME_H

#include "core.h"

typedef struct {
    float64 fps_limit;
    float64 current;
    float64 previous;
    float64 update;
    float64 render;
    float64 delta;
    float64 frame;
} Time;

static float64 time_get_total() {
    return glfwGetTime();
}

void app_init_timer(void);
void app_wait(float ms);
void app_set_fps_target(int fps);
float app_get_fps(void);

#endif