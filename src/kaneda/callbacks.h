#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "core.h"

// window callbacks -----------------------------

// GLFW3 WindowSize Callback, runs when window is resizedLastFrame
static inline void callback_window_size(GLFWwindow *window, int width, int height) {
    CORE.Window.render_size.width = width;
    CORE.Window.render_size.height = height;

    glViewport(CORE.Window.render_offset.x / 2, CORE.Window.render_offset.y / 2,
               CORE.Window.render_size.width - CORE.Window.render_offset.x,
               CORE.Window.render_size.height - CORE.Window.render_offset.y);

    CORE.Window.current_fbo.width = width;
    CORE.Window.current_fbo.height = height;
    CORE.Window.was_resized_last_frame = true;

    if (CORE.Window.is_fullscreen) {
        return;
    }

    CORE.Window.screen_size.width = width;
    CORE.Window.screen_size.height = height;
}

// GLFW3 WindowMaximize Callback, runs when window is maximized/restored
static inline void callback_window_maximize(GLFWwindow *window, int maximized) {
}

// GLFW3 WindowIconify Callback, runs when window is minimized/restored
static inline void callback_window_iconify(GLFWwindow *window, int iconified) {
}

// GLFW3 WindowFocus Callback, runs when window get/lose focus
static inline void callback_window_focus(GLFWwindow *window, int focused) {
}
// ----------------------------------------------

// mouse callbacks ------------------------------
static inline void callback_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    CORE.Input.Mouse.button_state_current[button] = action;
}

static inline void callback_cursor_pos(GLFWwindow *window, double x_pos, double y_pos) {
    CORE.Input.Mouse.position.x = (float)x_pos;
    CORE.Input.Mouse.position.y = (float)y_pos;
}

static inline void callback_mouse_scroll(GLFWwindow *window, double x_offset, double y_offset) {
    CORE.Input.Mouse.wheel_move_current = (float)y_offset;
}

static inline void callback_cursor_enter(GLFWwindow *window, int enter) {
    if (enter == true) {
        CORE.Input.Mouse.is_cursor_inside_client = true;
    } else {
        CORE.Input.Mouse.is_cursor_inside_client = false;
    }
}
// ----------------------------------------------

// keyboard callbacks ---------------------------
static inline void callback_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == CORE.Input.Keyboard.exit_key && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(CORE.Window.glfw_window, GLFW_TRUE);
    }

    if (action == GLFW_RELEASE) {
        CORE.Input.Keyboard.key_state_current[key] = 0;
    } else {
        CORE.Input.Keyboard.key_state_current[key] = 1;
    }

    // Check if there is space available in the key queue
    if ((CORE.Input.Keyboard.num_keys_pressed < MAX_KEYS_PRESSABLE) && (action == GLFW_RELEASE)) {
        // Add character to the queue
        CORE.Input.Keyboard.key_pressed_queue[CORE.Input.Keyboard.num_keys_pressed] = key;
        CORE.Input.Keyboard.num_keys_pressed++;
    }
}

static inline void callback_char(GLFWwindow *window, unsigned int key) {
    if (CORE.Input.Keyboard.num_chars_pressed < MAX_CHAR_PRESSED_QUEUE) {
        // Add character to the queue
        CORE.Input.Keyboard.char_pressed_queue[CORE.Input.Keyboard.num_chars_pressed] = key;
        CORE.Input.Keyboard.num_chars_pressed++;
    }
}
// ----------------------------------------------

#endif