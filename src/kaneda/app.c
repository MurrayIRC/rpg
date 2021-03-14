#include "app.h"
#include "input.h"
#include "window.h"
#include "renderer.h"
#include "shader.h"

#include "log.h"

bool app_run(void) {
    Window *win = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "rpg");
    if (win == NULL) {
        log_fatal("Failed to create window.\n");
        return false;
    }

    Renderer *rend = renderer_create(win);
    if (rend == NULL) {
        log_fatal("Couldn't create the renderer.\n");
        return false;
    }

    while (!glfwWindowShouldClose(win->glfw_window)) {
        // input
        // -----
        input_process(win);

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