#include "kaneda/kaneda.h"

#include <stdio.h>

void game_init() {
    log_info("game_init() called!\n");
}

void game_update(void) {
    // log_info("delta time: {%f}\n", time_delta());
}

void game_draw(void) {
}

void game_shutdown(void) {
    log_info("game_shutdown() called!\n");
}

int32_t main(int32_t argv, char **argc) {
    Game rpg = {.init = game_init,
                .update = game_update,
                .draw = game_draw,
                .shutdown = game_shutdown,
                .flags = {.is_fullscreen = false, .is_resizable = true, .vsync_on = true},
                .frame_rate = 60.0f,
                .window_width = 800,
                .window_height = 600,
                .window_title = "rpg"};

    Engine *e = engine_create(rpg);

    while (game_is_running()) {
        engine_frame();
    }

    free(e);
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
    Game rpg = {.init = game_init,
                .update = game_update,
                .draw = game_draw,
                .shutdown = game_shutdown,
                .flags = {.is_fullscreen = false, .is_resizable = true, .vsync_on = true},
                .frame_rate = 60.0f,
                .window_width = 800,
                .window_height = 600,
                .window_title = "rpg"};

    Engine *e = engine_create(rpg);

    while (game_is_running()) {
        engine_frame();
        log_info("hello again\n");
    }

    free(e);
    printf("Press enter to continue...\n");
    getchar();
    return 0;
}