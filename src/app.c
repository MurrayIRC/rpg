#include "app.h"

bool app_run() {
    window window = window_init(WIDTH, HEIGHT, "rpg");
    while (!window_should_close(window)) {
        glfwPollEvents();
    }
    
    return true;
}