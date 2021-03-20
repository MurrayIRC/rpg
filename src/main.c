#include "kaneda/kaneda.h"

#include <stdio.h>

void init() {
    log_info("init() called!\n");
}

void update(void) {
}

void draw(void) {
}

void shutdown(void) {
}

int32_t main(int32_t argv, char **argc) {
    Game game = {.init = init,
                 .update = update,
                 .draw = draw,
                 .shutdown = shutdown,
                 .flags = WINDOW_FLAGS_VSYNC,
                 .frame_rate = 60.0f,
                 .window_width = 800,
                 .window_height = 600,
                 .window_title = "rpg"};

    Engine *e = engine_create(game);

    while (engine_is_running()) {
        engine_frame();
    }

    free(e);

#ifdef PLATFORM_WINDOWS
    printf("Press enter to continue...\n");
    getchar();
#endif
    return 0;
}