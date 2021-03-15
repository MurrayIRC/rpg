#include "app.h"
#include "input.h"
#include "renderer.h"
#include "shader.h"

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

    if (!app_init_renderer(WINDOW_WIDTH, WINDOW_HEIGHT)) {
        log_fatal("Couldn't initialize the renderer.\n");
        return false;
    }

    while (!glfwWindowShouldClose(CORE.Window.glfw_window)) {
        // input
        // -----
        // input_process(win);

        // render
        // ------
        // renderer_draw_frame(rend);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(CORE.Window.glfw_window);
        glfwPollEvents();
    }

    // renderer_destroy(rend);
    // window_destroy(win);

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
    } else {
    }

    CORE.Window.glfw_window = glfwCreateWindow(w, h, name, NULL, NULL);
    if (CORE.Window.glfw_window == NULL) {
        log_fatal("Failed to create GLFW window.");
        return false;
    }
    glfwMakeContextCurrent(CORE.Window.glfw_window);

    // SET GLFW CALLBACKS FOR INPUT, WINDOW EVENTS, ETC.
    glfwSetFramebufferSizeCallback(CORE.Window.glfw_window, window_framebuffer_size_callback);
    glfwSetCursorPosCallback(CORE.Window.glfw_window, input_mouse_callback);
    glfwSetScrollCallback(CORE.Window.glfw_window, input_scroll_callback);

    // Tell GLFW to capture the mouse.
    // TODO: handle this in input
    // glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

bool app_init_renderer(const uint32 w, const uint32 h) {
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        log_fatal("Failed to initialize OpenGL context.\n");
        return false;
    }
    log_info("Loaded OpenGL %d.%d.\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    return true;
}

void window_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void app_shutdown(void) {
    glfwDestroyWindow(CORE.Window.glfw_window);
    glfwTerminate();
}