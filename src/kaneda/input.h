#ifndef INPUT_H
#define INPUT_H

#include "core.h"

void input_mouse_callback(GLFWwindow *window, double x_pos, double y_pos);
void input_scroll_callback(GLFWwindow *window, double x_offset, double y_offset);
void input_process(void);

#endif