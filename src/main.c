#include "kaneda/kaneda.h"

#include <stdio.h>

void game_init() {
    log_info("game_init() called!\n");
}

void game_update(void) {
}

void game_draw(void) {
}

void game_shutdown(void) {
    log_info("game_shutdown() called!\n");
}

int32_t main(int32_t argv, char **argc) {
    Game game = {.init = game_init,
                 .update = game_update,
                 .draw = game_draw,
                 .shutdown = game_shutdown,
                 .flags = WINDOW_FLAGS_VSYNC,
                 .frame_rate = 60.0f,
                 .window_width = 800,
                 .window_height = 600,
                 .window_title = "rpg"};

    Engine *e = engine_create(game);

    while (game_is_running()) {
        engine_frame();
    }

    free(e);

#ifdef PLATFORM_WINDOWS
    printf("Press enter to continue...\n");
    getchar();
#endif
    return 0;
}