#include "kaneda/kaneda.h"

#include <stdio.h>

void init() {
    log_info("init() called!\n");
}

void update() {
}

void draw() {
}

void shutdown() {
}

int32_t main(int32_t argv, char **argc) {
    Game g = {.init = init,
              .update = update,
              .draw = draw,
              .shutdown = shutdown,
              .flags = WINDOW_FLAGS_VSYNC,
              .frame_rate = 60.0f,
              .window_width = 800,
              .window_height = 600,
              .window_title = "rpg"};

    Engine *e = engine_create(g);
    while (engine_is_running()) {
        engine_frame();
    }

    free(e);

#ifdef PLATFORM_WIN
    printf("Press enter to continue...\n");
    getchar();
#endif
    return 0;
}