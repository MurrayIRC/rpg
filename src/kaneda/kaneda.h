#ifndef KANEDA_H
#define KANEDA_H

#include "core.h"

/*
███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗███████╗
██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██╔════╝
███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ███████╗
╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ╚════██║
███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ███████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝   ╚══════╝
*/

typedef struct Game {
    void (*init)();
    void (*update)();
    void (*draw)();
    void (*shutdown)();
    bool is_running;
    const char *window_title;
    uint32 window_width;
    uint32 window_height;
    float32 frame_rate;

    struct {
        bool is_resizable;
        bool is_fullscreen;
        bool vsync_on;
    } flags;
} Game;

typedef struct Input {
    struct {
        char key_state_current[512];  // registers current frame key-state
        char key_state_previous[512]; // registers previous frame key-state

        int32 key_pressed_queue[MAX_KEYS_PRESSABLE]; // Input keys queue
        uint32 num_keys_pressed;                     // Input keys queue count

        int32 char_pressed_queue[MAX_CHAR_PRESSED_QUEUE]; // Input characters queue
        uint32 num_chars_pressed;                         // Input characters queue count
    } keyboard;
    struct {
        vec2 position;

        int32 cursor; // tracks current mouse cursor.
        bool is_cursor_hidden;
        bool is_cursor_inside_client;

        char button_state_current[5];
        char button_state_previous[5];
        float wheel_move_current;  // registers current mouse wheel variation
        float wheel_move_previous; // registers previous mouse wheel variation
    } mouse;
    struct {
        int32 last_button_pressed;   // registers the last gamepad button pressed
        int32 num_available_axes;    // registers the number of available gamepad axes
        bool is_ready[MAX_GAMEPADS]; // flag for gamepad readiness
        float axis_state[MAX_GAMEPADS][MAX_GAMEPAD_AXES]; // gamepad axis states
        char button_state_current[MAX_GAMEPADS][MAX_GAMEPAD_AXES];
        char button_state_previous[MAX_GAMEPADS][MAX_GAMEPAD_AXES];
#ifdef PLATFORM_WINDOWS
        XINPUT_STATE xinput_state[MAX_GAMEPADS];
#endif
    } gamepad;
} Input;

typedef struct Time {
    float64 fps_limit;
    float64 current;
    float64 previous;
    float64 update;
    float64 render;
    float64 delta;
    float64 frame;
} Time;

typedef struct Platform {
    struct {
        GLFWwindow *handle;
        vec2i screen_size;
        vec2i display_size;
        vec2i render_size;
        vec2i position;
    } window;

    struct {
        LPCWSTR app_name;
        HINSTANCE hinstance;
        HWND hwnd;
        MSG msg;
    } win32;

    Input input;
    Time time;

    // Event *events;
    // Cursor *cursors;
} Platform;

typedef struct Audio {
} Audio;

typedef struct Graphics {
} Graphics;

typedef struct Engine {
    Platform *platform;
    Graphics *graphics;
    Audio *audio;
    Game game;
    void (*shutdown)();
} Engine;

// GLOBAL INSTANCE ---------------------
static Engine *engine_instance = {0};
Engine *engine(void) {
    return engine_instance;
}

Game *game(void) {
    return &engine()->game;
}

Input *input(void) {
    return &engine()->platform->input;
}

Time *time(void) {
    return &engine()->platform->time;
}
// -------------------------------------

/*
██████╗ ███████╗███████╗███████╗
██╔══██╗██╔════╝██╔════╝██╔════╝
██║  ██║█████╗  █████╗  ███████╗
██║  ██║██╔══╝  ██╔══╝  ╚════██║
██████╔╝███████╗██║     ███████║
╚═════╝ ╚══════╝╚═╝     ╚══════╝
*/

// GAME DEFINITIONS ------------------------
void game_default_function(void);
// -----------------------------------------

// ENGINE DEFINITIONS ----------------------
Engine *engine(void);
Game *game(void);
Engine *engine_create(Game game);
void engine_frame(void);
bool game_is_running(void);
void engine_sleep(float ms);
void engine_destroy(void);
// -----------------------------------------

// PLATFORM DEFINITIONS---------------------
extern Platform *platform_create(void);
extern void platform_open_window(const char *title, const uint32 width, const uint32 height);
extern void platform_update(Platform *platform);
extern void platform_destroy(Platform *platform);
extern void platform_window_add_flag(uint32 flag);
extern void platform_window_remove_flag(uint32 flag);
extern void platform_window_toggle_flag(uint32 flag);
extern bool platform_window_has_flag(uint32 flag);

extern bool is_fullscreen(void);
extern bool is_resizable(void);
extern bool is_vsync_on(void);
// -----------------------------------------

// GRAPHICS DEFINITIONS --------------------
extern Graphics *graphics_create();
extern void graphics_destroy(Graphics *graphics);
// -----------------------------------------

// AUDIO DEFINITIONS -----------------------
extern Audio *audio_create();
extern void audio_destroy(Audio *audio);
// -----------------------------------------

// INPUT DEFINITIONS -----------------------
extern void input_update(Input *input);
extern void input_process(Input *input);
extern bool input_is_key_pressed(int key);
extern bool input_is_key_down(int key);
extern bool input_is_key_released(int key);
extern bool input_is_key_up(int key);
extern int input_get_last_key_pressed(void);
extern int input_get_last_char_pressed(void);
extern float input_gamepad_get_axis_movement(int gamepad, int axis);
extern bool input_gamepad_is_button_pressed(int gamepad, int button);
extern bool input_gamepad_is_button_down(int gamepad, int button);
extern bool input_gamepad_is_button_released(int gamepad, int button);
extern bool input_gamepad_is_button_up(int gamepad, int button);
extern int input_gamepad_get_last_button_pressed(void);
extern int input_gamepad_set_mappings(const char *mappings);
extern bool input_mouse_is_button_pressed(int button);
extern bool input_mouse_is_button_down(int button);
extern bool input_mouse_is_button_released(int button);
extern bool input_mouse_is_button_up(int button);
extern int input_mouse_get_x(void);
extern int input_mouse_get_y(void);
extern vec2 input_mouse_get_position(void);
extern void input_mouse_set_position(int x, int y);
extern float input_mouse_get_wheel_move(void);
extern int input_mouse_get_cursor(void);
extern void input_mouse_set_cursor(int cursor);
// -----------------------------------------

// TIME DEFINITIONS ------------------------
extern float64 time_elapsed(void);
extern float64 time_get_fps(void);
extern float64 time_delta(void);
// -----------------------------------------

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

bool game_is_running(void) {
#ifdef PLATFORM_WINDOWS
    if (PeekMessage(&engine()->platform->win32.msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&engine()->platform->win32.msg);
        DispatchMessage(&engine()->platform->win32.msg);
    }

    return engine()->game.is_running && engine()->platform->win32.msg.message != WM_QUIT;
#else
    return engine()->game.is_running && !glfwWindowShouldClose(engine()->platform->window.handle);
#endif
}

/*
███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
█████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗
██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝
███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝
*/

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
        platform_open_window(game.window_title, game.window_width, game.window_height);
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
    if (!game_is_running()) {
        engine()->shutdown();
        return;
    }

    engine()->game.update();
    if (!game_is_running()) {
        engine()->shutdown();
        return;
    }

    engine()->game.draw();
    if (!game_is_running()) {
        engine()->shutdown();
        return;
    }

#ifdef PLATFORM_WINDOWS
#else
    glfwSwapBuffers(platform->window.handle);
#endif

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

/*
██╗███╗   ██╗████████╗███████╗██████╗ ███╗   ██╗ █████╗ ██╗
██║████╗  ██║╚══██╔══╝██╔════╝██╔══██╗████╗  ██║██╔══██╗██║
██║██╔██╗ ██║   ██║   █████╗  ██████╔╝██╔██╗ ██║███████║██║
██║██║╚██╗██║   ██║   ██╔══╝  ██╔══██╗██║╚██╗██║██╔══██║██║
██║██║ ╚████║   ██║   ███████╗██║  ██║██║ ╚████║██║  ██║███████╗
╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝
WHICH INCLUDES VARIOUS ENGINE SUBMODULES FROM OTHER FILES
*/

#include "platform.h"
#include "windows.h"
#include "graphics.h"
#include "audio.h"

#endif