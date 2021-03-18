#ifndef KANEDA_H
#define KANEDA_H

#include "platform.h"
#include "graphics.h"
#include "audio.h"

typedef struct {
    void (*init)();
    void (*update)();
    void (*draw)();
    void (*shutdown)();
    uint32_t flags;
    uint32_t window_width;
    uint32_t window_height;
    const char *window_title;
    float frame_rate;
} Game;

typedef struct {
    Platform *platform;
    Graphics *graphics;
    Audio *audio;
    Game game;
    void (*shutdown)();
} Engine;

void game_default_function();

Engine *engine();
Engine *engine_create(Game game);
void engine_frame();
bool engine_is_running();
void engine_destroy();

/*
 ██████╗  █████╗ ███╗   ███╗███████╗
██╔════╝ ██╔══██╗████╗ ████║██╔════╝
██║  ███╗███████║██╔████╔██║█████╗
██║   ██║██╔══██║██║╚██╔╝██║██╔══╝
╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗
 ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝
*/

void game_default_function() {
    // Empty for a reason...
}

/*
███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
█████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗
██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝
███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝
*/

static Engine *engine_instance = {0};
Engine *engine() {
    return engine_instance;
}

Engine *engine_create(Game game) {
    if (engine() == NULL) {
        // Check app desc for defaults
        if (game.window_width == 0) {
            game.window_width = 800;
        }
        if (game.window_height == 0) {
            game.window_height = 600;
        }
        if (game.window_title == 0) {
            game.window_title = "Kaneda Engine";
        }
        if (game.frame_rate <= 0.f) {
            game.frame_rate = 60.f;
        }
        if (game.update == NULL) {
            game.update = &game_default_function;
        }
        if (game.shutdown == NULL) {
            game.shutdown = &game_default_function;
        }
        if (game.init == NULL) {
            game.init = &game_default_function;
        }

        // Construct the instance of engine. There should only be one!!!!!
        engine_instance = kamalloc_init(Engine);
        engine()->game = game;

        // Set up function pointers
        engine()->shutdown = &engine_destroy;

        // Subsystem setup --------------------
        engine()->platform = platform_create();
        engine()->platform->time.fps_limit = game.frame_rate;
        platform_open_window(engine()->platform, game.window_title, game.window_width,
                             game.window_height, game.flags);
        engine()->graphics = graphics_create();
        engine()->audio = audio_create();
        // ------------------------------------

        // Call user game init function.
        game.init();
    }

    return engine();
}

void engine_frame() {
}

bool engine_is_running() {
    return !glfwWindowShouldClose(engine()->platform->window.handle);
}

void engine_destroy() {
    engine()->game.shutdown();

    graphics_destroy(engine()->graphics);
    audio_destroy(engine()->audio);
    platform_destroy(engine()->platform);
}

#endif