#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "core.h"

// window callbacks -----------------------------
static void glfw_error_callback(int error, const char *description) {
    log_warning("GLFW Error: %i Description: %s\n", error, description);
}

static LRESULT CALLBACK win32_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    if (umsg == WM_KEYDOWN) {
        input()->keyboard.key_state_current[(unsigned int)wparam] = 1;
    } else if (umsg == WM_KEYUP) {
        input()->keyboard.key_state_current[(unsigned int)wparam] = 0;
    } else {
        return DefWindowProc(hwnd, umsg, wparam, lparam);
    }

    // Check if there is space available in the key queue
    if ((input()->keyboard.num_keys_pressed < MAX_KEYS_PRESSABLE) && (umsg == WM_KEYUP)) {
        // Add character to the queue
        input()->keyboard.key_pressed_queue[input()->keyboard.num_keys_pressed] =
            (unsigned int)wparam;
        input()->keyboard.num_keys_pressed++;
    }

    return 0;
}

static LRESULT CALLBACK win32_wndproc_callback(HWND hwnd, UINT umessage, WPARAM wparam,
                                               LPARAM lparam) {
    switch (umessage) {
    // Check if the window is being destroyed.
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }

    // Check if the window is being closed.
    case WM_CLOSE: {
        PostQuitMessage(0);
        return 0;
    }

    // All other messages pass to the message handler in the system class.
    default: {
        return win32_message(hwnd, umessage, wparam, lparam);
    }
    }
}

// GLFW3 WindowSize Callback, runs when window is resizedLastFrame
static void callback_window_size(GLFWwindow *window, int width, int height) {
    Platform *platform = engine()->platform;

    platform->window.render_size.width = width;
    platform->window.render_size.height = height;
#ifdef PLATFORM_WINDOWS
    log_warning("Not Implemented: Callback Window Size\n");
#else
    glViewport(0, 0, platform->window.render_size.width, platform->window.render_size.height);
#endif

    // CORE.Window.current_fbo.width = width;
    // CORE.Window.current_fbo.height = height;
    // CORE.Window.was_resized_last_frame = true;

    // if (platform.window.is_fullscreen) return;

    platform->window.screen_size.width = width;
    platform->window.screen_size.height = height;
}

// GLFW3 WindowMaximize Callback, runs when window is maximized/restored
static void callback_window_maximize(GLFWwindow *window, int maximized) {
}

// GLFW3 WindowIconify Callback, runs when window is minimized/restored
static void callback_window_iconify(GLFWwindow *window, int iconified) {
}

// GLFW3 WindowFocus Callback, runs when window get/lose focus
static void callback_window_focus(GLFWwindow *window, int focused) {
}
// ----------------------------------------------

// mouse callbacks ------------------------------
static void callback_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    input()->mouse.button_state_current[button] = action;
}

static void callback_cursor_pos(GLFWwindow *window, double x_pos, double y_pos) {
    input()->mouse.position = math_vec2((float)x_pos, (float)y_pos);
}

static void callback_mouse_scroll(GLFWwindow *window, double x_offset, double y_offset) {
    input()->mouse.wheel_move_current = (float)y_offset;
}

static void callback_cursor_enter(GLFWwindow *window, int enter) {
    input()->mouse.is_cursor_inside_client = enter ? true : false;
}
// ----------------------------------------------

// keyboard callbacks ---------------------------
static void callback_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
        input()->keyboard.key_state_current[key] = 0;
    } else {
        input()->keyboard.key_state_current[key] = 1;
    }

    // Check if there is space available in the key queue
    if ((input()->keyboard.num_keys_pressed < MAX_KEYS_PRESSABLE) && (action == GLFW_RELEASE)) {
        // Add character to the queue
        input()->keyboard.key_pressed_queue[input()->keyboard.num_keys_pressed] = key;
        input()->keyboard.num_keys_pressed++;
    }
}

static void callback_char(GLFWwindow *window, unsigned int key) {
    if (input()->keyboard.num_chars_pressed < MAX_CHAR_PRESSED_QUEUE) {
        // Add character to the queue
        input()->keyboard.char_pressed_queue[input()->keyboard.num_chars_pressed] = key;
        input()->keyboard.num_chars_pressed++;
    }
}
// ----------------------------------------------

#endif