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

    while (!glfwWindowShouldClose(win->glfw_window)) {
        // input
        // -----
        window_process_input(win);

        // render
        // ------
        renderer_draw_frame(rend);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(win->glfw_window);
        glfwPollEvents();
    }

    renderer_destroy(rend);
    window_destroy(win);

    return true;
}