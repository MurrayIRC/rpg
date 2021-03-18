#include "app.h"
#include "renderer.h"
#include "callbacks.h"
#include "input.h"
#include "log.h"

static void glfw_error_callback(int error, const char *description) {
    log_warning("GLFW Error: %i Description: %s\n", error, description);
}

bool app_run(void) {
    if (!app_init_glfw()) {
        log_fatal("Couldn't initialize GLFW.\n");
        return false;
    }

    if (!app_init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "rpg")) {
        log_fatal("Couldn't initialize the window.\n");
        return false;
    }
    app_init_timer();

    if (!renderer_init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
        log_fatal("Couldn't initialize the renderer.\n");
        return false;
    }

    while (!glfwWindowShouldClose(CORE.Window.glfw_window)) {
        // input
        // -----
        // input_process(win);

        CORE.Time.current = glfwGetTime();
        CORE.Time.update = CORE.Time.current - CORE.Time.previous;
        CORE.Time.previous = CORE.Time.current;

        // RENDER HERE
        // ------
        renderer_draw_frame();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(CORE.Window.glfw_window);
        CORE.Time.current = glfwGetTime();
        CORE.Time.draw = CORE.Time.current - CORE.Time.previous;
        CORE.Time.previous = CORE.Time.current;
        CORE.Time.frame = CORE.Time.update + CORE.Time.draw;

        if (CORE.Time.frame < CORE.Time.target) {
            app_wait((float)(CORE.Time.target - CORE.Time.frame) * 1000.0f);

            CORE.Time.current = glfwGetTime();
            double wait_time = CORE.Time.current - CORE.Time.previous;
            CORE.Time.previous = CORE.Time.current;
            CORE.Time.frame += wait_time;
        }

        input_poll();
    }

    renderer_cleanup();

    return true;
}

bool app_init_glfw(void) {
    if (!glfwInit()) {
        log_fatal("Failed to initialize GLFW\n");
        return false;
    }

#ifdef PLATFORM_APPLE
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif

    glfwSetErrorCallback(glfw_error_callback);

    return true;
}

bool app_init_window(const uint32 w, const uint32 h, const char *name) {
    CORE.Input.Keyboard.exit_key = GLFW_KEY_ESCAPE;
    CORE.Input.Mouse.scale = math_vec2(1.0f, 1.0f);

    CORE.Window.screen_size.width = w;
    CORE.Window.screen_size.height = h;
    CORE.Window.screen_scale_matrix = math_mat4_identity();

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        log_fatal("GLFW failed to get the primary monitor.\n");
        return false;
    }
    // Get the total display size of the primary monitor.
    const GLFWvidmode *vid_mode = glfwGetVideoMode(monitor);
    CORE.Window.display_size.width = vid_mode->width;
    CORE.Window.display_size.height = vid_mode->height;

    // Screen size security check. Sets the window size to the full monitor size if window height
    // and width are unspecified.
    if (CORE.Window.screen_size.width == 0) {
        CORE.Window.screen_size.width = CORE.Window.display_size.width;
    }
    if (CORE.Window.screen_size.height == 0) {
        CORE.Window.screen_size.height = CORE.Window.display_size.height;
    }

    glfwDefaultWindowHints();
    // glfwWindowHint(GLFW_RED_BITS, 8);             // Framebuffer red color component bits
    // glfwWindowHint(GLFW_GREEN_BITS, 8);           // Framebuffer green color component bits
    // glfwWindowHint(GLFW_BLUE_BITS, 8);            // Framebuffer blue color component bits
    // glfwWindowHint(GLFW_ALPHA_BITS, 8);           // Framebuffer alpha color component bits
    // glfwWindowHint(GLFW_DEPTH_BITS, 24);          // Depthbuffer bits
    // glfwWindowHint(GLFW_REFRESH_RATE, 0);         // Refresh rate for fullscreen window
    // glfwWindowHint(GLFW_AUX_BUFFERS, 0);          // Number of auxiliar buffers

    // TODO: Window creation flags like fullscreen. These should be loaded from a settings file.
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    if (CORE.Window.is_fullscreen) {
        CORE.Window.position.x =
            CORE.Window.display_size.width / 2 - CORE.Window.screen_size.width / 2;
        CORE.Window.position.y =
            CORE.Window.display_size.height / 2 - CORE.Window.screen_size.height / 2;

        if (CORE.Window.position.x < 0) {
            CORE.Window.position.x = 0;
        }
        if (CORE.Window.position.y < 0) {
            CORE.Window.position.y = 0;
        }

        int32 num_modes = 0;
        const GLFWvidmode *modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &num_modes);
        // Get closest video mode to desired CORE.Window.screen.width/CORE.Window.screen.height
        for (int32 i = 0; i < num_modes; i++) {
            if ((uint32)modes[i].width >= CORE.Window.screen_size.width &&
                (uint32)modes[i].height >= CORE.Window.screen_size.height) {
                CORE.Window.display_size.width = modes[i].width;
                CORE.Window.display_size.height = modes[i].height;
            }
        }

        // If we are windowed fullscreen, ensures that window does not minimize when focus is lost
        if ((CORE.Window.screen_size.height == CORE.Window.display_size.height) &&
            (CORE.Window.screen_size.width == CORE.Window.display_size.width)) {
            glfwWindowHint(GLFW_AUTO_ICONIFY, 0);
        }

        log_warning("SYSTEM: Closest fullscreen videomode: %i x %i\n",
                    CORE.Window.display_size.width, CORE.Window.display_size.height);

        // SetupFramebuffer(CORE.Window.display_size.width, CORE.Window.display_size.height);

        CORE.Window.glfw_window = glfwCreateWindow(
            CORE.Window.display_size.width, CORE.Window.display_size.height,
            (CORE.Window.title != 0) ? CORE.Window.title : " ", glfwGetPrimaryMonitor(), NULL);
    } else {
        // No-fullscreen window creation
        CORE.Window.glfw_window =
            glfwCreateWindow(CORE.Window.screen_size.width, CORE.Window.screen_size.height,
                             (CORE.Window.title != 0) ? CORE.Window.title : " ", NULL, NULL);

        if (CORE.Window.glfw_window != NULL) {
            // Center window on screen
            int32 window_pos_x =
                CORE.Window.display_size.width / 2 - CORE.Window.screen_size.width / 2;
            int32 window_pos_y =
                CORE.Window.display_size.height / 2 - CORE.Window.screen_size.height / 2;

            if (window_pos_x < 0) {
                window_pos_x = 0;
            }
            if (window_pos_y < 0) {
                window_pos_y = 0;
            }

            glfwSetWindowPos(CORE.Window.glfw_window, window_pos_x, window_pos_y);
            CORE.Window.render_size.width = CORE.Window.screen_size.width;
            CORE.Window.render_size.height = CORE.Window.screen_size.height;
        }
    }

    // MAKE SURE THE WINDOW WAS CREATED
    if (CORE.Window.glfw_window == NULL) {
        log_fatal("Failed to create GLFW window.\n");
        return false;
    }

    log_info("DISPLAY: Device initialized successfully.\n");
    log_info("    > Display size: %i x %i\n", CORE.Window.display_size.width,
             CORE.Window.display_size.height);
    log_info("    > Render size:  %i x %i\n", CORE.Window.render_size.width,
             CORE.Window.render_size.height);
    log_info("    > Screen size:  %i x %i\n", CORE.Window.screen_size.width,
             CORE.Window.screen_size.height);
    log_info("    > Viewport offsets: %i, %i\n", CORE.Window.render_offset.x,
             CORE.Window.render_offset.y);

    // SET GLFW CALLBACKS FOR INPUT, WINDOW EVENTS, ETC.
    // Set window callback events
    // NOTE: Resizing not allowed by default!
    glfwSetWindowSizeCallback(CORE.Window.glfw_window, callback_window_size);
    glfwSetWindowMaximizeCallback(CORE.Window.glfw_window, callback_window_maximize);
    glfwSetWindowIconifyCallback(CORE.Window.glfw_window, callback_window_iconify);
    glfwSetWindowFocusCallback(CORE.Window.glfw_window, callback_window_focus);
    // Set input callback events
    glfwSetKeyCallback(CORE.Window.glfw_window, callback_key);
    glfwSetCharCallback(CORE.Window.glfw_window, callback_char);
    // Set mouse callback events
    glfwSetMouseButtonCallback(CORE.Window.glfw_window, callback_mouse_button);
    glfwSetCursorPosCallback(CORE.Window.glfw_window, callback_cursor_pos);
    glfwSetScrollCallback(CORE.Window.glfw_window, callback_mouse_scroll);
    glfwSetCursorEnterCallback(CORE.Window.glfw_window, callback_cursor_enter);

    glfwMakeContextCurrent(CORE.Window.glfw_window);
    glfwSwapInterval(CORE.Window.vsync);

    int32 fb_width = CORE.Window.render_size.width;
    int32 fb_height = CORE.Window.render_size.height;

    if (CORE.Window.high_dpi == true) {
        glfwGetFramebufferSize(CORE.Window.glfw_window, &fb_width, &fb_height);
        // scaling matrix required in case desired screen area is different than display area
        CORE.Window.screen_scale_matrix =
            math_scale(math_vec3((float)fb_width / CORE.Window.screen_size.width,
                                 (float)fb_height / CORE.Window.screen_size.height, 1.0f));
#if !defined(PLATFORM_APPLE)
        // SetMouseScale((float)CORE.Window.screen.width/fbWidth,
        // (float)CORE.Window.screen.height/fbHeight);
#endif
    }

    CORE.Window.render_size.width = fb_width;
    CORE.Window.render_size.height = fb_height;
    CORE.Window.current_fbo.width = CORE.Window.screen_size.width;
    CORE.Window.current_fbo.height = CORE.Window.screen_size.height;

    // ClearBackground(RAYWHITE);

    return true;
}

void app_init_timer(void) {
    srand((uint32)time(NULL));

    CORE.Time.previous = glfwGetTime();
}

int app_get_current_monitor(void) {
    int num_monitors;
    GLFWmonitor **monitors = glfwGetMonitors(&num_monitors);
    GLFWmonitor *monitor = NULL;

    if (num_monitors == 1) {
        return 0;
    }

    if (CORE.Window.is_fullscreen) {
        monitor = glfwGetWindowMonitor(CORE.Window.glfw_window);
        for (int i = 0; i < num_monitors; i++) {
            if (monitors[i] == monitor) {
                return i;
            }
        }
        return 0;
    } else {
        int x = 0;
        int y = 0;
        glfwGetWindowPos(CORE.Window.glfw_window, &x, &y);

        for (int i = 0; i < num_monitors; i++) {
            int mx = 0;
            int my = 0;
            int width = 0;
            int height = 0;
            monitor = monitors[i];
            glfwGetMonitorWorkarea(monitor, &mx, &my, &width, &height);
            if (x >= mx && x <= (mx + width) && y >= my && y <= (my + height)) {
                return i;
            }
        }
    }
    return 0;
}

void app_wait(float ms) {
#ifdef PLATFORM_WINDOWS
    Sleep((unsigned int)ms);
#endif
#ifdef PLATFORM_APPLE
    usleep(ms * 1000.0f);
#endif
#ifdef PLATFORM_LINUX
    struct timespec req = {0};
    time_t sec = (int)(ms / 1000.0f);
    ms -= (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = ms * 1000000L;
    while (nanosleep(&req, &req) == -1) {
        continue;
    }
#endif
}

void app_set_fps_target(int fps) {
    if (fps < 1) {
        CORE.Time.target = 0.0;
    } else {
        CORE.Time.target = 1.0 / (double)fps;
    }
}

float app_get_fps(void) {
#define FPS_CAPTURE_FRAMES_COUNT 30   // 30 captures
#define FPS_AVERAGE_TIME_SECONDS 0.5f // 500 millisecondes
#define FPS_STEP (FPS_AVERAGE_TIME_SECONDS / FPS_CAPTURE_FRAMES_COUNT)
    static int index = 0;
    static float history[FPS_CAPTURE_FRAMES_COUNT] = {0};
    static float average = 0, last = 0;
    float fps_frame = (float)CORE.Time.frame;

    if (fps_frame == 0) {
        return 0;
    }

    if ((glfwGetTime() - last) > FPS_STEP) {
        last = (float)glfwGetTime();
        index = (index + 1) % FPS_CAPTURE_FRAMES_COUNT;
        average -= history[index];
        history[index] = fps_frame / FPS_CAPTURE_FRAMES_COUNT;
        average += history[index];
    }

    return roundf(1.0f / average);
}

void app_window_toggle_fullscreen(void) {
    if (!CORE.Window.is_fullscreen) {
        // Store previous window position (in case we exit fullscreen)
        glfwGetWindowPos(CORE.Window.glfw_window, &CORE.Window.position.x, &CORE.Window.position.y);

        int num_monitors = 0;
        GLFWmonitor **monitors = glfwGetMonitors(&num_monitors);

        int monitor_index = app_get_current_monitor();
        // use GetCurrentMonitor so we correctly get the display the window is on
        GLFWmonitor *monitor = monitor_index < num_monitors ? monitors[monitor_index] : NULL;

        if (!monitor) {
            log_warning("GLFW: Failed to get monitor.\n");

            CORE.Window.is_fullscreen = false; // Toggle fullscreen flag
            // CORE.Window.flags &= ~FLAG_FULLSCREEN_MODE;

            glfwSetWindowMonitor(CORE.Window.glfw_window, NULL, 0, 0, CORE.Window.screen_size.width,
                                 CORE.Window.screen_size.height, GLFW_DONT_CARE);
            return;
        }

        CORE.Window.is_fullscreen = true; // Toggle fullscreen flag
        // CORE.Window.flags |= FLAG_FULLSCREEN_MODE;

        glfwSetWindowMonitor(CORE.Window.glfw_window, monitor, 0, 0, CORE.Window.screen_size.width,
                             CORE.Window.screen_size.height, GLFW_DONT_CARE);
    } else {
        CORE.Window.is_fullscreen = false; // Toggle fullscreen flag
        // CORE.Window.flags &= ~FLAG_FULLSCREEN_MODE;

        glfwSetWindowMonitor(CORE.Window.glfw_window, NULL, CORE.Window.position.x,
                             CORE.Window.position.y, CORE.Window.screen_size.width,
                             CORE.Window.screen_size.height, GLFW_DONT_CARE);
    }

    if (CORE.Window.vsync) {
        glfwSwapInterval(1);
    }
}

void app_window_maximize(void) {
    if (glfwGetWindowAttrib(CORE.Window.glfw_window, GLFW_RESIZABLE) == GLFW_TRUE) {
        glfwMaximizeWindow(CORE.Window.glfw_window);
        // CORE.Window.flags |= FLAG_WINDOW_MAXIMIZED;
    }
}

void app_window_minimize(void) {
    glfwIconifyWindow(CORE.Window.glfw_window);
}

void app_window_set_title(const char *title) {
    CORE.Window.title = title;
    glfwSetWindowTitle(CORE.Window.glfw_window, title);
}

void app_shutdown(void) {
    glfwDestroyWindow(CORE.Window.glfw_window);
    glfwTerminate();
}