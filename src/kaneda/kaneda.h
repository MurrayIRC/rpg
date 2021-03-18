#ifndef KANEDA_H
#define KANEDA_H

#include "core.h"
#include "platform.h"
#include "graphics.h"
#include "audio.h"

typedef struct {
    void (*init)();
    void (*update)();
    void (*draw)();
    void (*shutdown)();
    bool is_running;
    const char *window_title;
    uint32 window_width;
    uint32 window_height;
    uint32 flags;
    float32 frame_rate;
} Game;

typedef struct {
    Platform *platform;
    Graphics *graphics;
    Audio *audio;
    Game game;
    void (*shutdown)();
} Engine;

void game_default_function(void);

Engine *engine(void);
Game *engine_game(void);

Engine *engine_create(Game game);
void engine_frame(void);
bool engine_is_running(void);
void engine_sleep(float ms);
void engine_destroy(void);

/*
 ██████╗  █████╗ ███╗   ███╗███████╗
██╔════╝ ██╔══██╗████╗ ████║██╔════╝
██║  ███╗███████║██╔████╔██║█████╗
██║   ██║██╔══██║██║╚██╔╝██║██╔══╝
╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗
 ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝
*/

void game_default_function(void) {
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
Engine *engine(void) {
    return engine_instance;
}

Game *engine_game(void) {
    return &engine()->game;
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
        platform_open_window(game.window_title, game.window_width, game.window_height, game.flags);
        engine()->graphics = graphics_create();
        engine()->audio = audio_create();
        // ------------------------------------

        // Call user game init function.
        game.init();
        engine()->game.is_running = true;
    }

    return engine();
}

void engine_frame(void) {
    Platform *platform = engine()->platform;

    platform->time.current = time_elapsed();
    platform->time.update = platform->time.current - platform->time.previous;
    platform->time.previous = platform->time.current;

    platform_update(platform);
    if (!engine_is_running()) {
        engine()->shutdown();
        return;
    }

    engine()->game.update();
    if (!engine_is_running()) {
        engine()->shutdown();
        return;
    }

    engine()->game.draw();
    if (!engine_is_running()) {
        engine()->shutdown();
        return;
    }

    glfwSwapBuffers(platform->window.handle);

    platform->time.current = time_elapsed();
    platform->time.render = platform->time.current - platform->time.previous;
    platform->time.previous = platform->time.current;
    platform->time.frame = platform->time.update + platform->time.render;
    platform->time.delta = platform->time.frame / 1000.0;

    float target = 1000.0 / platform->time.fps_limit;
    if (platform->time.frame < target) {
        engine_sleep((float)(target - platform->time.frame));
        platform->time.current = time_elapsed();
        float64 wait_time = platform->time.current - platform->time.previous;
        platform->time.previous = platform->time.current;
        platform->time.frame += wait_time;
        platform->time.delta = platform->time.frame / 1000.0;
    }
}

bool engine_is_running(void) {
    return engine()->game.is_running;
}

void engine_sleep(float ms) {
#ifdef PLATFORM_WINDOWS
    Sleep((unsigned int)ms);
#endif
#ifdef PLATFORM_OSX
    usleep(ms * 1000.0f);
#endif
#ifdef PLATFORM_LINUX
    struct timespec req = {0};
    time_t sec = (int)(ms / 1000.0f);
    ms -= (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = ms * 1000000L;
    while (nanosleep(&req, &req) == -1) {
        continue;
    }
#endif
}

void engine_destroy(void) {
    engine()->game.shutdown();
    engine()->game.is_running = false;

    graphics_destroy(engine()->graphics);
    audio_destroy(engine()->audio);
    platform_destroy(engine()->platform);
}

#endif