#include "app.h"
#include "renderer.h"
#include "shader.h"
#include "window.h"

#include "log.h"

bool app_run(void) {
    Window *win = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "rpg");
    if (win == NULL) {
        log_fatal("Failed to create window.");
        return false;
    }

    Renderer *rend = renderer_create(win);

    while (!window_should_close(win)) {
        glfwPollEvents();
        renderer_draw_frame(rend);
        glfwSwapBuffers(win->glfw_window);
    }

    renderer_destroy(rend);
    window_destroy(win);

    return true;
}