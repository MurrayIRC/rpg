#include "app.h"
#include "window.h"
#include "log.h"

bool app_run(void) {
    Window *win = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "rpg");
    if (win == NULL) {
        log_fatal("Failed to create window.");
        return false;
    }

    if (!gladLoadGL()) {
        log_fatal("Failed to initialize GLAD.");
        return false;
    }

    while (!window_should_close(win)) {
        glfwPollEvents();
    }
    
    window_destroy(win);
    
    return true;
}