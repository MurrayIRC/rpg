/* credit for this module lies entirely with Eskil Steenberg. Some of his code has been adapted here
 * for my purposes, with some minor convenience edits. see http://gamepipeline.org/ for more. */

#ifndef CORE_H
#define CORE_H

/* Platform Apple */
#if (defined __APPLE__ || defined _APPLE)
#define PLATFORM_APPLE
/* Platform Windows */
#elif (defined _WIN32 || defined _WIN64)
// Necessary windows defines before including windows.h.
#define OEMRESOURCE
#define PLATFORM_WIN
#include <windows.h>
/* Platform Linux */
#elif (defined linux || defined _linux || defined __linux__)
#define PLATFORM_LINUX
/* Platform Emscripten */
#elif (defined __EMSCRIPTEN__)
#define PLATFORM_WEB
/* Else - Platform Undefined and Unsupported */
#endif

#if (defined PLATFORM_APPLE || defined PLATFORM_LINUX)
#include <sched.h>
#include <unistd.h>
#elif (defined PLATFORM_WIN)
#include <windows.h>
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>

#ifdef PLATFORM_WIN
typedef unsigned int uint;
#else
#include <sys/stat.h>
#include <sys/types.h>

#endif

#include <stdint.h>
typedef size_t usize;
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef float float32;
typedef double float64;

/* Defines PI */
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923

#ifndef NO_MEMORY_DEBUG // currently broken w/ stb_image :(
//#define MEMORY_DEBUG /* turns on the memory debugging system */
#endif
#ifndef EXIT_CRASH
//#define EXIT_CRASH /* turns on crash on exit */
#endif

#ifdef MEMORY_DEBUG
/* ----- Debugging -----
If MEMORY_DEBUG  is enabled, the memory debugging system will create macros that replace malloc,
free and realloc and allows the system to kept track of and report where memory is being allocated,
how much and if the memory is being fred. This is very useful for finding memory leaks in large
applications. The system can also over allocate memory and fill it with a magic number and can
therfor detect if the application writes outside of the allocated memory. if EXIT_CRASH is defined,
then exit(); will be replaced with a funtion that writes to NULL. This will make it trivial ti find
out where an application exits using any debugger., */

extern void debug_memory_init(void (*lock)(void *mutex), void (*unlock)(void *mutex),
                              void *mutex); /* Required for memory debugger to be thread safe */
extern void *debug_mem_malloc(
    uint size, char *file,
    uint line); /* Replaces malloc and records the c file and line where it was called*/
extern void *debug_mem_realloc(
    void *pointer, uint size, char *file,
    uint line); /* Replaces realloc and records the c file and line where it was called*/
extern void
debug_mem_free(void *buf); /* Replaces free and records the c file and line where it was called*/
extern void debug_mem_print(
    uint min_allocs); /* Prints out a list of all allocations made, their location, how much memorey
                         each has allocated, freed, and how many allocations have been made. The
                         min_allocs parameter can be set to avoid printing any allocations that have
                         been made fewer times then min_allocs */
extern void debug_mem_reset(
    void); /* debug_mem_reset allows you to clear all memory stored in the debugging system if you
              only want to record allocations after a specific point in your code*/
extern bool debug_memory(
    void); /* debug_memory checks if any of the bounds of any allocation has been over written and
              reports where to standard out. The function returns TRUE if any error was found*/

#define malloc(n) debug_mem_malloc(n, __FILE__, __LINE__)         /* Replaces malloc. */
#define realloc(n, m) debug_mem_realloc(n, m, __FILE__, __LINE__) /* Replaces realloc. */
#define free(n) debug_mem_free(n)                                 /* Replaces free. */

#endif

#ifdef EXIT_CRASH
extern void exit_crash(uint i); /* function guaranteed to crash (Writes to NULL).*/
#define exit(n) exit_crash(n)   /* overwriting exit(0) with a function guaraneed to crash. */
#endif

#ifndef MAX_GAMEPADS
#define MAX_GAMEPADS 4 // Max number of gamepads supported
#endif
#ifndef MAX_GAMEPAD_AXES
#define MAX_GAMEPAD_AXES 8 // Max number of axes supported (per gamepad)
#endif
#ifndef MAX_GAMEPAD_BUTTONS
#define MAX_GAMEPAD_BUTTONS 32 // Max number of buttons supported (per gamepad)
#endif
#ifndef MAX_CHAR_PRESSED_QUEUE
#define MAX_CHAR_PRESSED_QUEUE 16 // Max number of characters in the char input queue
#endif
#ifndef MAX_KEYS_PRESSABLE
#define MAX_KEYS_PRESSABLE 16 // Max number of keys in the key input queue
#endif

#include "math.h"

typedef struct CoreData {
    struct {
        GLFWwindow *glfw_window;
        const char *title;

        uint32 flags; // configuration flags for the window (bit-based)
        bool is_fullscreen;
        bool should_close;
        bool was_resized_last_frame;
        bool vsync;
        bool high_dpi;

        vec2i position;     // window position on screen
        vec2i display_size; // display width & height (monitor, device screen, etc)
        vec2i screen_size;  // screen width & height (used render area)
        vec2i current_fbo; // Current render width and height, it could change on BeginTextureMode()
        vec2i render_size; // framebuffer width & height
        vec2i render_offset;      // Offset from render area (must be divided by 2)
        mat4 screen_scale_matrix; // for framebuffer rendering
    } Window;
    struct {
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        unsigned int tex_container;
        unsigned int tex_face;

        vec3 cube_pos;
    } Renderer;
    struct {
        struct {
            int32 exit_key;               // default exit key
            char key_state_current[512];  // registers current frame key-state
            char key_state_previous[512]; // registers previous frame key-state

            int32 key_pressed_queue[MAX_KEYS_PRESSABLE]; // Input keys queue
            uint32 num_keys_pressed;                     // Input keys queue count

            int32 char_pressed_queue[MAX_CHAR_PRESSED_QUEUE]; // Input characters queue
            uint32 num_chars_pressed;                         // Input characters queue count
        } Keyboard;
        struct {
            vec2 position;
            vec2 offset;
            vec2 scale;

            int32 cursor; // tracks current mouse cursor.
            bool is_cursor_hidden;
            bool is_cursor_inside_client;

            char button_state_current[5];
            char button_state_previous[5];
            float wheel_move_current;  // registers current mouse wheel variation
            float wheel_move_previous; // registers previous mouse wheel variation
        } Mouse;
        struct {
            int32 last_button_pressed;   // registers the last gamepad button pressed
            int32 num_available_axes;    // registers the number of available gamepad axes
            bool is_ready[MAX_GAMEPADS]; // flag for gamepad readiness
            float axis_state[MAX_GAMEPADS][MAX_GAMEPAD_AXES]; // gamepad axis states
            char button_state_current[MAX_GAMEPADS][MAX_GAMEPAD_AXES];
            char button_state_previous[MAX_GAMEPADS][MAX_GAMEPAD_AXES];
        } Gamepad;
    } Input;
    struct {
        double current;  // Current time measure
        double previous; // Previous time measure
        double update;   // Time measure for frame update
        double draw;     // Time measure for frame draw
        double frame;    // Time measure for one frame
        double target;   // Desired time for one frame, if 0 not applied
    } Time;
} CoreData;

static CoreData CORE = {0};

#define kamalloc(__SZ) malloc(__SZ)
static inline void *_kamalloc_init_impl(size_t sz) {
    void *data = kamalloc(sz);
    memset(data, 0, sz);
    return data;
}
#define kamalloc_init(__TYPE) (__TYPE *)_kamalloc_init_impl(sizeof(__TYPE))

#endif