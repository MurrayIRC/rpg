#ifndef PLATFORM_H
#define PLATFORM_H

#include "core.h"
#include "input.h"
#include "time.h"
#include "callbacks.h"

#define WINDOW_FLAGS_NO_RESIZE 0x01
#define WINDOW_FLAGS_FULLSCREEN 0x02
#define WINDOW_FLAGS_VSYNC 0x03

typedef struct {
    struct {
        GLFWwindow *handle;
        vec2 screen_size;
        vec2 display_size;
        vec2 render_size;
        vec2 position;
    } window;

    Input input;
    Time time;

    // Event *events;
    // Cursor *cursors;
} Platform;

Platform *platform_create() {
    Platform *platform = kamalloc_init(Platform);

    assert(platform);

    glfwInit();

#ifdef PLATFORM_APPLE
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

    glfwSetErrorCallback(glfw_error_callback);

    return platform;
}

void platform_open_window(Platform *platform, const char *title, const uint32 width,
                          const uint32 height, uint32 flags) {
    platform->window.screen_size.width = width;
    platform->window.screen_size.height = height;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *vid_mode = glfwGetVideoMode(monitor);
    platform->window.display_size.width = vid_mode->width;
    platform->window.display_size.height = vid_mode->height;

    glfwWindowHint(GLFW_RESIZABLE, (flags & WINDOW_FLAGS_NO_RESIZE) != WINDOW_FLAGS_NO_RESIZE);

    if ((flags & WINDOW_FLAGS_FULLSCREEN) != WINDOW_FLAGS_FULLSCREEN) {
        platform->window.position.x =
            platform->window.display_size.width / 2 - platform->window.screen_size.width / 2;
        platform->window.position.y =
            platform->window.display_size.height / 2 - platform->window.screen_size.height / 2;

        if (platform->window.position.x < 0) {
            platform->window.position.x = 0;
        }
        if (platform->window.position.y < 0) {
            platform->window.position.y = 0;
        }

        int32 num_modes = 0;
        const GLFWvidmode *modes = glfwGetVideoModes(monitor, &num_modes);
        // Get closest video mode to desired
        // platform->window.screen_size.width/platform->window.screen.height
        for (int32 i = 0; i < num_modes; i++) {
            if ((uint32)modes[i].width >= platform->window.screen_size.width &&
                (uint32)modes[i].height >= platform->window.screen_size.height) {
                platform->window.display_size.width = modes[i].width;
                platform->window.display_size.height = modes[i].height;
            }
        }

        // If we are windowed fullscreen, ensures that window does not minimize when focus is lost
        if ((platform->window.screen_size.height == platform->window.display_size.height) &&
            (platform->window.screen_size.width == platform->window.display_size.width)) {
            glfwWindowHint(GLFW_AUTO_ICONIFY, 0);
        }

        log_info("Closest fullscreen videomode: %i x %i\n", platform->window.display_size.width,
                 platform->window.display_size.height);

        platform->window.handle = glfwCreateWindow(platform->window.display_size.width,
                                                   platform->window.display_size.height, title,
                                                   glfwGetPrimaryMonitor(), NULL);
    } else {
        // No-fullscreen window creation
        platform->window.handle =
            glfwCreateWindow(platform->window.screen_size.width,
                             platform->window.screen_size.height, title, NULL, NULL);

        if (platform->window.handle != NULL) {
            // Center window on screen
            int32 pos_x =
                platform->window.display_size.width / 2 - platform->window.screen_size.width / 2;
            int32 pos_y =
                platform->window.display_size.height / 2 - platform->window.screen_size.height / 2;

            if (pos_x < 0) {
                pos_x = 0;
            }
            if (pos_y < 0) {
                pos_y = 0;
            }

            glfwSetWindowPos(platform->window.handle, pos_x, pos_y);
            platform->window.render_size.width = platform->window.screen_size.width;
            platform->window.render_size.height = platform->window.screen_size.height;
        }
    }

    // MAKE SURE THE WINDOW WAS CREATED
    if (platform->window.handle == NULL) {
        log_fatal("Failed to create GLFW window.\n");
        glfwTerminate();
        return;
    }

    log_info("DISPLAY: Device initialized successfully.\n");
    log_info("    > Display size: %i x %i\n", platform->window.display_size.width,
             platform->window.display_size.height);
    log_info("    > Render size:  %i x %i\n", platform->window.render_size.width,
             platform->window.render_size.height);
    log_info("    > Screen size:  %i x %i\n", platform->window.screen_size.width,
             platform->window.screen_size.height);

    glfwMakeContextCurrent(platform->window.handle);

    // SET GLFW CALLBACKS FOR INPUT, WINDOW EVENTS, ETC.
    glfwSetWindowSizeCallback(platform->window.handle, callback_window_size);
    glfwSetWindowMaximizeCallback(platform->window.handle, callback_window_maximize);
    glfwSetWindowIconifyCallback(platform->window.handle, callback_window_iconify);
    glfwSetWindowFocusCallback(platform->window.handle, callback_window_focus);
    // Set input callback events
    glfwSetKeyCallback(platform->window.handle, callback_key);
    glfwSetCharCallback(platform->window.handle, callback_char);
    // Set mouse callback events
    glfwSetMouseButtonCallback(platform->window.handle, callback_mouse_button);
    glfwSetCursorPosCallback(platform->window.handle, callback_cursor_pos);
    glfwSetScrollCallback(platform->window.handle, callback_mouse_scroll);
    glfwSetCursorEnterCallback(platform->window.handle, callback_cursor_enter);

    glfwSwapInterval((flags & WINDOW_FLAGS_VSYNC) != WINDOW_FLAGS_VSYNC);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        log_fatal("Failed to initialize OpenGL context.\n");
        return;
    }
    log_info("Loaded OpenGL %d.%d.\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
}

void platform_destroy(Platform *platform) {
    if (platform == NULL) {
        return;
    }

    glfwDestroyWindow(platform->window.handle);
    glfwTerminate();

    free(platform);
    platform = NULL;
}

#endif