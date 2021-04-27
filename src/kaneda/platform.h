#ifndef PLATFORM_H
#define PLATFORM_H

#include "core.h"
#include "callbacks.h"

typedef enum KeyboardKey {
    // Alphanumeric keys
    KEY_APOSTROPHE = 39,
    KEY_COMMA = 44,
    KEY_MINUS = 45,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_ZERO = 48,
    KEY_ONE = 49,
    KEY_TWO = 50,
    KEY_THREE = 51,
    KEY_FOUR = 52,
    KEY_FIVE = 53,
    KEY_SIX = 54,
    KEY_SEVEN = 55,
    KEY_EIGHT = 56,
    KEY_NINE = 57,
    KEY_SEMICOLON = 59,
    KEY_EQUAL = 61,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,

    // Function keys
    KEY_SPACE = 32,
    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_PAGE_UP = 266,
    KEY_PAGE_DOWN = 267,
    KEY_HOME = 268,
    KEY_END = 269,
    KEY_CAPS_LOCK = 280,
    KEY_SCROLL_LOCK = 281,
    KEY_NUM_LOCK = 282,
    KEY_PRINT_SCREEN = 283,
    KEY_PAUSE = 284,
    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_SHIFT = 344,
    KEY_RIGHT_CONTROL = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,
    KEY_KB_MENU = 348,
    KEY_LEFT_BRACKET = 91,
    KEY_BACKSLASH = 92,
    KEY_RIGHT_BRACKET = 93,
    KEY_GRAVE = 96,

    // Keypad keys
    KEY_KP_0 = 320,
    KEY_KP_1 = 321,
    KEY_KP_2 = 322,
    KEY_KP_3 = 323,
    KEY_KP_4 = 324,
    KEY_KP_5 = 325,
    KEY_KP_6 = 326,
    KEY_KP_7 = 327,
    KEY_KP_8 = 328,
    KEY_KP_9 = 329,
    KEY_KP_DECIMAL = 330,
    KEY_KP_DIVIDE = 331,
    KEY_KP_MULTIPLY = 332,
    KEY_KP_SUBTRACT = 333,
    KEY_KP_ADD = 334,
    KEY_KP_ENTER = 335,
    KEY_KP_EQUAL = 336
} KeyboardKey;

typedef enum MouseButton {
    MOUSE_LEFT_BUTTON = 0,
    MOUSE_RIGHT_BUTTON = 1,
    MOUSE_MIDDLE_BUTTON = 2
} MouseButton;

typedef enum MouseCursor {
    MOUSE_CURSOR_DEFAULT = 0,
    MOUSE_CURSOR_ARROW = 1,
    MOUSE_CURSOR_IBEAM = 2,
    MOUSE_CURSOR_CROSSHAIR = 3,
    MOUSE_CURSOR_POINTING_HAND = 4,
    MOUSE_CURSOR_RESIZE_EW = 5,   // The horizontal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NS = 6,   // The vertical resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NWSE = 7, // The top-left to bottom-right diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NESW = 8, // The top-right to bottom-left diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_ALL = 9,  // The omni-directional resize/move cursor shape
    MOUSE_CURSOR_NOT_ALLOWED = 10 // The operation-not-allowed shape
} MouseCursor;

typedef enum GamepadNumber {
    GAMEPAD_PLAYER1 = 0,
    GAMEPAD_PLAYER2 = 1,
    GAMEPAD_PLAYER3 = 2,
    GAMEPAD_PLAYER4 = 3
} GamepadNumber;

typedef enum GamepadButton {
    // This is here just for error checking
    GAMEPAD_BUTTON_UNKNOWN = 0,

    GAMEPAD_BUTTON_DPAD_UP,
    GAMEPAD_BUTTON_DPAD_DOWN,
    GAMEPAD_BUTTON_DPAD_LEFT,
    GAMEPAD_BUTTON_DPAD_RIGHT,
    GAMEPAD_BUTTON_Y_OR_TRIANGLE,
    GAMEPAD_BUTTON_A_OR_CROSS,
    GAMEPAD_BUTTON_X_OR_SQUARE,
    GAMEPAD_BUTTON_B_OR_CIRCLE,
    GAMEPAD_BUTTON_LEFT_BUMPER,
    GAMEPAD_BUTTON_LEFT_TRIGGER,
    GAMEPAD_BUTTON_RIGHT_BUMPER,
    GAMEPAD_BUTTON_RIGHT_TRIGGER,
    GAMEPAD_BUTTON_LEFT_THUMB,
    GAMEPAD_BUTTON_RIGHT_THUMB,
    GAMEPAD_BUTTON_SELECT,     // PS3 Select
    GAMEPAD_BUTTON_PS_OR_XBOX, // PS Button/XBOX Button
    GAMEPAD_BUTTON_START       // PS3 Start
} GamepadButton;

typedef enum GamepadAxis {
    // Left stick
    GAMEPAD_AXIS_LEFT_X = 0,
    GAMEPAD_AXIS_LEFT_Y = 1,

    // Right stick
    GAMEPAD_AXIS_RIGHT_X = 2,
    GAMEPAD_AXIS_RIGHT_Y = 3,

    // Pressure levels for the back triggers
    GAMEPAD_AXIS_LEFT_TRIGGER = 4, // [1..-1] (pressure-level)
    GAMEPAD_AXIS_RIGHT_TRIGGER = 5 // [1..-1] (pressure-level)
} GamepadAxis;

/*
██████╗ ██╗      █████╗ ████████╗███████╗ ██████╗ ██████╗ ███╗   ███╗
██╔══██╗██║     ██╔══██╗╚══██╔══╝██╔════╝██╔═══██╗██╔══██╗████╗ ████║
██████╔╝██║     ███████║   ██║   █████╗  ██║   ██║██████╔╝██╔████╔██║
██╔═══╝ ██║     ██╔══██║   ██║   ██╔══╝  ██║   ██║██╔══██╗██║╚██╔╝██║
██║     ███████╗██║  ██║   ██║   ██║     ╚██████╔╝██║  ██║██║ ╚═╝ ██║
╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝      ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝
*/

Platform *platform_create(void) {
    Platform *platform = kamalloc_init(Platform);

    assert(platform);

#ifdef PLATFORM_WINDOWS

#else
    glfwInit();

#ifdef PLATFORM_OSX
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

    glfwSetErrorCallback(glfw_error_callback);
#endif

    return platform;
}

void platform_open_window(const char *title, const uint32 width, const uint32 height) {
    Platform *platform = engine()->platform;

    platform->window.screen_size.width = width;
    platform->window.screen_size.height = height;

#ifdef PLATFORM_WINDOWS
    platform->win32.hinstance = GetModuleHandle(NULL);
    platform->win32.app_name = title;
    ZeroMemory(&platform->win32.msg, sizeof(MSG));

    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = win32_wndproc_callback;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = platform->win32.hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = platform->win32.app_name;
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);
    platform->window.display_size.width = GetSystemMetrics(SM_CXSCREEN);
    platform->window.display_size.height = GetSystemMetrics(SM_CYSCREEN);

    DEVMODE dm_screen_settings;
    if (is_fullscreen()) {
        memset(&dm_screen_settings, 0, sizeof(dm_screen_settings));
        dm_screen_settings.dmSize = sizeof(dm_screen_settings);
        dm_screen_settings.dmPelsWidth = (unsigned long)platform->window.display_size.width;
        dm_screen_settings.dmPelsHeight = (unsigned long)platform->window.display_size.height;
        dm_screen_settings.dmBitsPerPel = 32;
        dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN);
        platform->window.position.x = platform->window.position.y = 0;

        platform->win32.hwnd = CreateWindowEx(
            WS_EX_APPWINDOW, title, title, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
            platform->window.position.x, platform->window.position.y,
            platform->window.display_size.width, platform->window.display_size.height, NULL, NULL,
            platform->win32.hinstance, NULL);
    } else {
        platform->window.position.x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
        platform->window.position.y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

        platform->win32.hwnd = CreateWindowEx(
            WS_EX_APPWINDOW, title, title, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
            platform->window.position.x, platform->window.position.y,
            platform->window.screen_size.width, platform->window.screen_size.height, NULL, NULL,
            platform->win32.hinstance, NULL);
    }

    ShowWindow(platform->win32.hwnd, SW_SHOW);
    SetForegroundWindow(platform->win32.hwnd);
    SetFocus(platform->win32.hwnd);
    ShowCursor(true);

#else
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *vid_mode = glfwGetVideoMode(monitor);
    platform->window.display_size.width = vid_mode->width;
    platform->window.display_size.height = vid_mode->height;

    glfwWindowHint(GLFW_RESIZABLE, is_resizable());

    // glfwWindowHint(GLFW_RESIZABLE, (flags & WINDOW_FLAGS_RESIZABLE) != WINDOW_FLAGS_RESIZABLE);

    if (is_fullscreen()) {
        platform->window.position.x =
            platform->window.display_size.width / 2 - platform->window.screen_size.width / 2;
        platform->window.position.y =
            platform->window.display_size.height / 2 - platform->window.screen_size.height / 2;

        if (platform->window.position.x < 0) {
            platform->window.position.x = 0;
        }
        if (platform->window.position.y < 0) {
            platform->window.position.y = 0;
        }

        int32 num_modes = 0;
        const GLFWvidmode *modes = glfwGetVideoModes(monitor, &num_modes);
        // Get closest video mode to desired
        // platform->window.screen_size.width/platform->window.screen.height
        for (int32 i = 0; i < num_modes; i++) {
            if ((uint32)modes[i].width >= platform->window.screen_size.width &&
                (uint32)modes[i].height >= platform->window.screen_size.height) {
                platform->window.display_size.width = modes[i].width;
                platform->window.display_size.height = modes[i].height;
            }
        }

        // If we are windowed fullscreen, ensures that window does not minimize when focus is lost
        if ((platform->window.screen_size.height == platform->window.display_size.height) &&
            (platform->window.screen_size.width == platform->window.display_size.width)) {
            glfwWindowHint(GLFW_AUTO_ICONIFY, 0);
        }

        log_info("Closest fullscreen videomode: %i x %i\n", platform->window.display_size.width,
                 platform->window.display_size.height);

        platform->window.handle = glfwCreateWindow(platform->window.display_size.width,
                                                   platform->window.display_size.height, title,
                                                   glfwGetPrimaryMonitor(), NULL);
    } else {
        // No-fullscreen window creation
        platform->window.handle =
            glfwCreateWindow(platform->window.screen_size.width,
                             platform->window.screen_size.height, title, NULL, NULL);

        if (platform->window.handle != NULL) {
            // Center window on screen
            int32 pos_x =
                platform->window.display_size.width / 2 - platform->window.screen_size.width / 2;
            int32 pos_y =
                platform->window.display_size.height / 2 - platform->window.screen_size.height / 2;

            if (pos_x < 0) {
                pos_x = 0;
            }
            if (pos_y < 0) {
                pos_y = 0;
            }

            glfwSetWindowPos(platform->window.handle, pos_x, pos_y);
            platform->window.render_size.width = platform->window.screen_size.width;
            platform->window.render_size.height = platform->window.screen_size.height;
        }
    }

    // MAKE SURE THE WINDOW WAS CREATED
    if (platform->window.handle == NULL) {
        log_fatal("Failed to create GLFW window.\n");
        glfwTerminate();
        return;
    }

    log_info("DISPLAY: Device initialized successfully.\n");
    log_info("    > Display size: %i x %i\n", platform->window.display_size.width,
             platform->window.display_size.height);
    log_info("    > Render size:  %i x %i\n", platform->window.render_size.width,
             platform->window.render_size.height);
    log_info("    > Screen size:  %i x %i\n", platform->window.screen_size.width,
             platform->window.screen_size.height);

    glfwMakeContextCurrent(platform->window.handle);

    // SET GLFW CALLBACKS FOR INPUT, WINDOW EVENTS, ETC.
    glfwSetWindowSizeCallback(platform->window.handle, callback_window_size);
    glfwSetWindowMaximizeCallback(platform->window.handle, callback_window_maximize);
    glfwSetWindowIconifyCallback(platform->window.handle, callback_window_iconify);
    glfwSetWindowFocusCallback(platform->window.handle, callback_window_focus);
    // Set input callback events
    glfwSetKeyCallback(platform->window.handle, callback_key);
    glfwSetCharCallback(platform->window.handle, callback_char);
    // Set mouse callback events
    glfwSetMouseButtonCallback(platform->window.handle, callback_mouse_button);
    glfwSetCursorPosCallback(platform->window.handle, callback_cursor_pos);
    glfwSetScrollCallback(platform->window.handle, callback_mouse_scroll);
    glfwSetCursorEnterCallback(platform->window.handle, callback_cursor_enter);

    glfwSwapInterval(is_vsync_on());

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        log_fatal("Failed to initialize OpenGL context.\n");
        return;
    }
    log_info("Loaded OpenGL %d.%d.\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#endif
}

void platform_update(Platform *platform) {
    input_update(&platform->input);
    input_process(&platform->input);
    // events_poll();
}

void platform_destroy(Platform *platform) {
    if (platform == NULL) {
        return;
    }

#ifdef PLATFORM_WINDOWS
    ShowCursor(true);
    if (is_fullscreen()) {
        ChangeDisplaySettings(NULL, 0);
    }

    DestroyWindow(platform->win32.hwnd);
    platform->win32.hwnd = NULL;
    UnregisterClass(platform->win32.app_name, platform->win32.hinstance);
    platform->win32.hinstance = NULL;
#else
    glfwDestroyWindow(platform->window.handle);
    glfwTerminate();
#endif

    free(platform);
    platform = NULL;
}

bool is_fullscreen(void) {
    return engine()->game.flags.is_fullscreen;
}

bool is_resizable(void) {
    return engine()->game.flags.is_resizable;
}

bool is_vsync_on(void) {
    return engine()->game.flags.vsync_on;
}

/*
██╗███╗   ██╗██████╗ ██╗   ██╗████████╗
██║████╗  ██║██╔══██╗██║   ██║╚══██╔══╝
██║██╔██╗ ██║██████╔╝██║   ██║   ██║
██║██║╚██╗██║██╔═══╝ ██║   ██║   ██║
██║██║ ╚████║██║     ╚██████╔╝   ██║
╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝    ╚═╝
*/

void input_update(Input *input) {
    engine()->platform->input.keyboard.num_keys_pressed = 0;
    engine()->platform->input.keyboard.num_chars_pressed = 0;

    // Register previous keys states
    for (int i = 0; i < 512; i++) {
        engine()->platform->input.keyboard.key_state_previous[i] =
            engine()->platform->input.keyboard.key_state_current[i];
    }

    // Register previous mouse states
    for (int i = 0; i < 3; i++) {
        engine()->platform->input.mouse.button_state_previous[i] =
            engine()->platform->input.mouse.button_state_current[i];
    }

    // Register previous mouse wheel state
    engine()->platform->input.mouse.wheel_move_previous =
        engine()->platform->input.mouse.wheel_move_current;
    engine()->platform->input.mouse.wheel_move_current = 0.0f;

    for (int i = 0; i < MAX_GAMEPADS; i++) {
#ifdef PLATFORM_WINDOWS
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        engine()->platform->input.gamepad.is_ready[i] =
            XInputGetState((DWORD)i, &state) == ERROR_SUCCESS ? true : false;

        engine()->platform->input.gamepad.xinput_state[i] = state;
#else
        engine()->platform->input.gamepad.is_ready[i] = glfwJoystickPresent(i) ? true : false;
#endif
    }
}

void input_process(Input *input) {
    // Register gamepads buttons events
    for (int i = 0; i < MAX_GAMEPADS; i++) {
        if (engine()->platform->input.gamepad.is_ready[i]) {
            // Register previous gamepad states
            for (int k = 0; k < MAX_GAMEPAD_BUTTONS; k++)
                engine()->platform->input.gamepad.button_state_previous[i][k] =
                    engine()->platform->input.gamepad.button_state_current[i][k];

                // Get current gamepad state
                // NOTE: There is no callback available, so we get it manually
                // Get remapped buttons
#ifdef PLATFORM_WINDOWS
            XINPUT_GAMEPAD gamepad = engine()->platform->input.gamepad.xinput_state[i].Gamepad;
            const WORD buttons = gamepad.wButtons;

            GamepadButton button = -1;

            if (buttons & XINPUT_GAMEPAD_DPAD_UP) {
                button = GAMEPAD_BUTTON_DPAD_UP;
            } else if (buttons & XINPUT_GAMEPAD_DPAD_DOWN) {
                button = GAMEPAD_BUTTON_DPAD_DOWN;
            } else if (buttons & XINPUT_GAMEPAD_DPAD_LEFT) {
                button = GAMEPAD_BUTTON_DPAD_LEFT;
            } else if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT) {
                button = GAMEPAD_BUTTON_DPAD_RIGHT;
            } else if (buttons & XINPUT_GAMEPAD_START) {
                button = GAMEPAD_BUTTON_START;
            } else if (buttons & XINPUT_GAMEPAD_BACK) {
                button = GAMEPAD_BUTTON_SELECT;
            } else if (buttons & XINPUT_GAMEPAD_LEFT_THUMB) {
                button = GAMEPAD_BUTTON_LEFT_THUMB;
            } else if (buttons & XINPUT_GAMEPAD_RIGHT_THUMB) {
                button = GAMEPAD_BUTTON_RIGHT_THUMB;
            } else if (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
                button = GAMEPAD_BUTTON_LEFT_BUMPER;
            } else if (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
                button = GAMEPAD_BUTTON_RIGHT_BUMPER;
            } else if (buttons & XINPUT_GAMEPAD_A) {
                button = GAMEPAD_BUTTON_A_OR_CROSS;
            } else if (buttons & XINPUT_GAMEPAD_B) {
                button = GAMEPAD_BUTTON_B_OR_CIRCLE;
            } else if (buttons & XINPUT_GAMEPAD_X) {
                button = GAMEPAD_BUTTON_X_OR_SQUARE;
            } else if (buttons & XINPUT_GAMEPAD_Y) {
                button = GAMEPAD_BUTTON_Y_OR_TRIANGLE;
            }

            if (button != -1) {
                engine()->platform->input.gamepad.button_state_current[i][button] = 1;
                engine()->platform->input.gamepad.last_button_pressed = button;

                // TODO: Figure out how to get button state current to set to 0 if not pressed.
            }

            // TODO: no magic numbers!!!
            float deadzone_x = 0.05f;
            float deadzone_y = 0.05f;

            float norm_lx = fmaxf(-1, (float)gamepad.sThumbLX / 32767);
            float norm_ly = fmaxf(-1, (float)gamepad.sThumbLY / 32767);
            float norm_rx = fmaxf(-1, (float)gamepad.sThumbRX / 32767);
            float norm_ry = fmaxf(-1, (float)gamepad.sThumbRY / 32767);
            float lx = (abs(norm_lx) < deadzone_x
                            ? 0
                            : (abs(norm_lx) - deadzone_x) * (norm_lx / abs(norm_lx)));

            float ly = (abs(norm_ly) < deadzone_y
                            ? 0
                            : (abs(norm_ly) - deadzone_y) * (norm_ly / abs(norm_ly)));
            float rx = (abs(norm_rx) < deadzone_x
                            ? 0
                            : (abs(norm_rx) - deadzone_x) * (norm_rx / abs(norm_rx)));
            float ry = (abs(norm_ry) < deadzone_y
                            ? 0
                            : (abs(norm_ry) - deadzone_y) * (norm_ry / abs(norm_ry)));

            if (deadzone_x > 0) {
                lx *= 1 / (1 - deadzone_x);
                rx *= 1 / (1 - deadzone_x);
            }
            if (deadzone_y > 0) {
                ly *= 1 / (1 - deadzone_y);
                ry *= 1 / (1 - deadzone_y);
            }

            engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_LEFT_X] = lx;
            engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_LEFT_Y] = ly;
            engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_RIGHT_X] = rx;
            engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_RIGHT_Y] = ry;
            engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_LEFT_TRIGGER] =
                gamepad.bLeftTrigger / 255;
            engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_RIGHT_TRIGGER] =
                gamepad.bRightTrigger / 255;

            // Register buttons for 2nd triggers (because GLFW doesn't count these as buttons
            // but rather axis)
            // TODO: NO MAGIC NUMBERS IDIOT
            engine()->platform->input.gamepad.button_state_current[i][GAMEPAD_BUTTON_LEFT_TRIGGER] =
                (char)(engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_LEFT_TRIGGER] >
                       0.1);
            engine()
                ->platform->input.gamepad.button_state_current[i][GAMEPAD_BUTTON_RIGHT_TRIGGER] =
                (char)(engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_RIGHT_TRIGGER] >
                       0.1);

            engine()->platform->input.gamepad.num_available_axes = GLFW_GAMEPAD_AXIS_LAST;
#else
            GLFWgamepadstate state = {0};
            glfwGetGamepadState(i, &state); // This remaps all gamepads so they have their buttons
                                            // mapped like an xbox controller
            const unsigned char *buttons = state.buttons;

            for (int k = 0; (buttons != NULL) && (k < GLFW_GAMEPAD_BUTTON_LAST + 1) &&
                            (k < MAX_GAMEPAD_BUTTONS);
                 k++) {
                GamepadButton button = -1;

                switch (k) {
                case GLFW_GAMEPAD_BUTTON_Y:
                    button = GAMEPAD_BUTTON_Y_OR_TRIANGLE;
                    break;
                case GLFW_GAMEPAD_BUTTON_B:
                    button = GAMEPAD_BUTTON_B_OR_CIRCLE;
                    break;
                case GLFW_GAMEPAD_BUTTON_A:
                    button = GAMEPAD_BUTTON_A_OR_CROSS;
                    break;
                case GLFW_GAMEPAD_BUTTON_X:
                    button = GAMEPAD_BUTTON_X_OR_SQUARE;
                    break;

                case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                    button = GAMEPAD_BUTTON_LEFT_BUMPER;
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                    button = GAMEPAD_BUTTON_RIGHT_BUMPER;
                    break;

                case GLFW_GAMEPAD_BUTTON_BACK:
                    button = GAMEPAD_BUTTON_SELECT;
                    break;
                case GLFW_GAMEPAD_BUTTON_GUIDE:
                    button = GAMEPAD_BUTTON_PS_OR_XBOX;
                    break;
                case GLFW_GAMEPAD_BUTTON_START:
                    button = GAMEPAD_BUTTON_START;
                    break;

                case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                    button = GAMEPAD_BUTTON_DPAD_UP;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                    button = GAMEPAD_BUTTON_DPAD_RIGHT;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                    button = GAMEPAD_BUTTON_DPAD_DOWN;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
                    button = GAMEPAD_BUTTON_DPAD_LEFT;
                    break;

                case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
                    button = GAMEPAD_BUTTON_LEFT_THUMB;
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
                    button = GAMEPAD_BUTTON_RIGHT_THUMB;
                    break;
                default:
                    break;
                }

                if (button != -1) // Check for valid button
                {
                    if (buttons[k] == GLFW_PRESS) {
                        engine()->platform->input.gamepad.button_state_current[i][button] = 1;
                        engine()->platform->input.gamepad.last_button_pressed = button;
                    } else
                        engine()->platform->input.gamepad.button_state_current[i][button] = 0;
                }
            }

            // Get current axis state
            const float *axes = state.axes;

            for (int k = 0;
                 (axes != NULL) && (k < GLFW_GAMEPAD_AXIS_LAST + 1) && (k < MAX_GAMEPAD_AXES);
                 k++) {
                engine()->platform->input.gamepad.axis_state[i][k] = axes[k];
            }

            // Register buttons for 2nd triggers (because GLFW doesn't count these as buttons
            // but rather axis)
            engine()->platform->input.gamepad.button_state_current[i][GAMEPAD_BUTTON_LEFT_TRIGGER] =
                (char)(engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_LEFT_TRIGGER] >
                       0.1);
            engine()
                ->platform->input.gamepad.button_state_current[i][GAMEPAD_BUTTON_RIGHT_TRIGGER] =
                (char)(engine()->platform->input.gamepad.axis_state[i][GAMEPAD_AXIS_RIGHT_TRIGGER] >
                       0.1);

            engine()->platform->input.gamepad.num_available_axes = GLFW_GAMEPAD_AXIS_LAST;
#endif
        }
    }

    CORE.Window.was_resized_last_frame = false;

#ifndef PLATFORM_WINDOWS
    glfwPollEvents();
#endif
}

// Detect if a key has been pressed once
bool input_is_key_pressed(int key) {
    return (engine()->platform->input.keyboard.key_state_previous[key] == 0) &&
           (engine()->platform->input.keyboard.key_state_current[key] == 1);
}

// Detect if a key is being pressed (key held down)
bool input_is_key_down(int key) {
    return engine()->platform->input.keyboard.key_state_current[key] == 1;
}

// Detect if a key has been released once
bool input_is_key_released(int key) {
    return (engine()->platform->input.keyboard.key_state_previous[key] == 1) &&
           (engine()->platform->input.keyboard.key_state_current[key] == 0);
}

// Detect if a key is NOT being pressed (key not held down)
bool input_is_key_up(int key) {
    return engine()->platform->input.keyboard.key_state_current[key] == 0;
}

// Get the last key pressed
int input_get_last_key_pressed(void) {
    int value = 0;

    if (engine()->platform->input.keyboard.num_keys_pressed > 0) {
        // Get character from the queue head
        value = engine()->platform->input.keyboard.key_pressed_queue[0];

        // Shift elements 1 step toward the head.
        for (int i = 0; i < (engine()->platform->input.keyboard.num_keys_pressed - 1); i++) {
            engine()->platform->input.keyboard.key_pressed_queue[i] =
                engine()->platform->input.keyboard.key_pressed_queue[i + 1];
        }

        // Reset last character in the queue
        engine()
            ->platform->input.keyboard
            .key_pressed_queue[engine()->platform->input.keyboard.num_keys_pressed] = 0;
        engine()->platform->input.keyboard.num_keys_pressed--;
    }

    return value;
}

// Get the last char pressed
int input_get_last_char_pressed(void) {
    int value = 0;

    if (engine()->platform->input.keyboard.num_chars_pressed > 0) {
        // Get character from the queue head
        value = engine()->platform->input.keyboard.char_pressed_queue[0];

        // Shift elements 1 step toward the head.
        for (int i = 0; i < (engine()->platform->input.keyboard.num_chars_pressed - 1); i++) {
            engine()->platform->input.keyboard.char_pressed_queue[i] =
                engine()->platform->input.keyboard.char_pressed_queue[i + 1];
        }

        // Reset last character in the queue
        engine()
            ->platform->input.keyboard
            .char_pressed_queue[engine()->platform->input.keyboard.num_chars_pressed] = 0;
        engine()->platform->input.keyboard.num_chars_pressed--;
    }

    return value;
}

// Return axis movement vector for a gamepad
float input_gamepad_get_axis_movement(int gamepad, int axis) {
    if ((gamepad < MAX_GAMEPADS) && engine()->platform->input.gamepad.is_ready[gamepad] &&
        (axis < MAX_GAMEPAD_AXES) &&
        (fabsf(engine()->platform->input.gamepad.axis_state[gamepad][axis]) > 0.1f)) {
        return engine()->platform->input.gamepad.axis_state[gamepad][axis];
    }

    return 0.0f;
}

// Detect if a gamepad button has been pressed once
bool input_gamepad_is_button_pressed(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && engine()->platform->input.gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (engine()->platform->input.gamepad.button_state_previous[gamepad][button] == 0) &&
           (engine()->platform->input.gamepad.button_state_current[gamepad][button] == 1);
}

// Detect if a gamepad button is being pressed
bool input_gamepad_is_button_down(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && engine()->platform->input.gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (engine()->platform->input.gamepad.button_state_current[gamepad][button] == 1);
}

// Detect if a gamepad button has NOT been pressed once
bool input_gamepad_is_button_released(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && engine()->platform->input.gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (engine()->platform->input.gamepad.button_state_previous[gamepad][button] == 1) &&
           (engine()->platform->input.gamepad.button_state_current[gamepad][button] == 0);
}

// Detect if a mouse button is NOT being pressed
bool input_gamepad_is_button_up(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && engine()->platform->input.gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (engine()->platform->input.gamepad.button_state_current[gamepad][button] == 0);
}

// Get the last gamepad button pressed
int input_gamepad_get_last_button_pressed(void) {
    return engine()->platform->input.gamepad.last_button_pressed;
}

// Set internal gamepad mappings
int input_gamepad_set_mappings(const char *mappings) {
#ifdef PLATFORM_WINDOWS
    log_warning("Not Implemented: Gamepad Set Mappings.\n");
#else
    return glfwUpdateGamepadMappings(mappings);
#endif
}

// Detect if a mouse button has been pressed once
bool input_mouse_is_button_pressed(int button) {
    return (engine()->platform->input.mouse.button_state_current[button] == 1) &&
           (engine()->platform->input.mouse.button_state_previous[button] == 0);
}

// Detect if a mouse button is being pressed
bool input_mouse_is_button_down(int button) {
    return engine()->platform->input.mouse.button_state_current[button] == 1;
}

// Detect if a mouse button has been released once
bool input_mouse_is_button_released(int button) {
    return (engine()->platform->input.mouse.button_state_current[button] == 0) &&
           (engine()->platform->input.mouse.button_state_previous[button] == 1);
}

// Detect if a mouse button is NOT being pressed
bool input_mouse_is_button_up(int button) {
    return !input_mouse_is_button_down(button);
}

// Returns mouse position X
int input_mouse_get_x(void) {
    return (int)(engine()->platform->input.mouse.position.x);
}

// Returns mouse position Y
int input_mouse_get_y(void) {
    return (int)(engine()->platform->input.mouse.position.y);
}

// Returns mouse position XY
vec2 input_mouse_get_position(void) {
    return math_vec2(engine()->platform->input.mouse.position.x,
                     engine()->platform->input.mouse.position.y);
}

// Set mouse position XY
void input_mouse_set_position(int x, int y) {
    engine()->platform->input.mouse.position = math_vec2((float)x, (float)y);

#ifdef PLATFORM_WINDOWS
    SetCursorPos(engine()->platform->input.mouse.position.x,
                 engine()->platform->input.mouse.position.y);
#else
    glfwSetCursorPos(CORE.Window.glfw_window, engine()->platform->input.mouse.position.x,
                     engine()->platform->input.mouse.position.y);
#endif
}

// Returns mouse wheel movement Y
float input_mouse_get_wheel_move(void) {
    return engine()->platform->input.mouse.wheel_move_previous;
}

// Returns mouse cursor
int input_mouse_get_cursor(void) {
    return engine()->platform->input.mouse.cursor;
}

// Set mouse cursor
// NOTE: This is a no-op on platforms other than PLATFORM_DESKTOP
void input_mouse_set_cursor(int cursor) {
    engine()->platform->input.mouse.cursor = cursor;
    if (cursor == MOUSE_CURSOR_DEFAULT)
#ifdef PLATFORM_WINDOWS
        log_warning("Unimplemented: Set Cursor.\n");
#else
        glfwSetCursor(CORE.Window.glfw_window, NULL);
#endif

    else {
// NOTE: We are relating internal GLFW enum values to our MouseCursor enum values
#ifdef PLATFORM_WINDOWS
        log_warning("Unimplemented: Set Cursor.\n");
#else
        glfwSetCursor(CORE.Window.glfw_window, glfwCreateStandardCursor(0x00036000 + cursor));
#endif
    }
}

/*
████████╗██╗███╗   ███╗███████╗
╚══██╔══╝██║████╗ ████║██╔════╝
   ██║   ██║██╔████╔██║█████╗
   ██║   ██║██║╚██╔╝██║██╔══╝
   ██║   ██║██║ ╚═╝ ██║███████╗
   ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝
*/

float64 time_elapsed(void) {
#ifdef PLATFORM_WINDOWS
    INT64 current_time;
    QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
    return current_time;
#else
    return glfwGetTime() * 1000.0;
#endif
}

float64 time_get_fps(void) {
#define FPS_CAPTURE_FRAMES_COUNT 30   // 30 captures
#define FPS_AVERAGE_TIME_SECONDS 0.5f // 500 millisecondes
#define FPS_STEP (FPS_AVERAGE_TIME_SECONDS / FPS_CAPTURE_FRAMES_COUNT)
    static int index = 0;
    static float64 history[FPS_CAPTURE_FRAMES_COUNT] = {0};
    static float64 average = 0, last = 0;

    float64 fps_frame = engine()->platform->time.frame;

    if (fps_frame == 0) {
        return 0;
    }

    float64 time = time_elapsed() / 1000.0;

    if ((time - last) > FPS_STEP) {
        last = (float)time;
        index = (index + 1) % FPS_CAPTURE_FRAMES_COUNT;
        average -= history[index];
        history[index] = fps_frame / FPS_CAPTURE_FRAMES_COUNT;
        average += history[index];
    }

    return round(1.0 / average);
}

float64 time_delta(void) {
    return engine()->platform->time.delta;
}

#endif