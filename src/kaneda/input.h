#ifndef INPUT_H
#define INPUT_H

#include "core.h"

// Keyboard keys (US keyboard layout)
// NOTE: Use GetKeyPressed() to allow redefining
// required keys for alternative layouts
typedef enum {
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

// Mouse buttons
typedef enum { MOUSE_LEFT_BUTTON = 0, MOUSE_RIGHT_BUTTON = 1, MOUSE_MIDDLE_BUTTON = 2 } MouseButton;

// Mouse cursor types
typedef enum {
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

// Gamepad number
typedef enum {
    GAMEPAD_PLAYER1 = 0,
    GAMEPAD_PLAYER2 = 1,
    GAMEPAD_PLAYER3 = 2,
    GAMEPAD_PLAYER4 = 3
} GamepadNumber;

// Gamepad buttons
typedef enum {
    // This is here just for error checking
    GAMEPAD_BUTTON_UNKNOWN = 0,

    // This is normally a DPAD
    GAMEPAD_BUTTON_LEFT_FACE_UP,
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT,
    GAMEPAD_BUTTON_LEFT_FACE_DOWN,
    GAMEPAD_BUTTON_LEFT_FACE_LEFT,

    // This normally corresponds with PlayStation and Xbox controllers
    // XBOX: [Y,X,A,B]
    // PS3: [Triangle,Square,Cross,Circle]
    // No support for 6 button controllers though..
    GAMEPAD_BUTTON_RIGHT_FACE_UP,
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN,
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT,

    // Triggers
    GAMEPAD_BUTTON_LEFT_TRIGGER_1,
    GAMEPAD_BUTTON_LEFT_TRIGGER_2,
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1,
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2,

    // These are buttons in the center of the gamepad
    GAMEPAD_BUTTON_MIDDLE_LEFT,  // PS3 Select
    GAMEPAD_BUTTON_MIDDLE,       // PS Button/XBOX Button
    GAMEPAD_BUTTON_MIDDLE_RIGHT, // PS3 Start

    // These are the joystick press in buttons
    GAMEPAD_BUTTON_LEFT_THUMB,
    GAMEPAD_BUTTON_RIGHT_THUMB
} GamepadButton;

// Gamepad axis
typedef enum {
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

static inline void input_poll(void) {
    CORE.Input.Keyboard.num_keys_pressed = 0;
    CORE.Input.Keyboard.num_chars_pressed = 0;

    // Register previous keys states
    for (int i = 0; i < 512; i++) {
        CORE.Input.Keyboard.key_state_previous[i] = CORE.Input.Keyboard.key_state_current[i];
    }

    // Register previous mouse states
    for (int i = 0; i < 3; i++) {
        CORE.Input.Mouse.button_state_previous[i] = CORE.Input.Mouse.button_state_current[i];
    }

    // Register previous mouse wheel state
    CORE.Input.Mouse.wheel_move_previous = CORE.Input.Mouse.wheel_move_current;
    CORE.Input.Mouse.wheel_move_current = 0.0f;

    for (int i = 0; i < MAX_GAMEPADS; i++) {
        CORE.Input.Gamepad.is_ready[i] = glfwJoystickPresent(i) ? true : false;
    }

    // Register gamepads buttons events
    for (int i = 0; i < MAX_GAMEPADS; i++) {
        if (CORE.Input.Gamepad.is_ready[i]) {
            // Register previous gamepad states
            for (int k = 0; k < MAX_GAMEPAD_BUTTONS; k++)
                CORE.Input.Gamepad.button_state_previous[i][k] =
                    CORE.Input.Gamepad.button_state_current[i][k];

            // Get current gamepad state
            // NOTE: There is no callback available, so we get it manually
            // Get remapped buttons
            GLFWgamepadstate state = {0};
            glfwGetGamepadState(i, &state); // This remapps all gamepads so they have their
                                            // buttons mapped like an xbox controller

            const unsigned char *buttons = state.buttons;

            for (int k = 0; (buttons != NULL) && (k < GLFW_GAMEPAD_BUTTON_DPAD_LEFT + 1) &&
                            (k < MAX_GAMEPAD_BUTTONS);
                 k++) {
                GamepadButton button = -1;

                switch (k) {
                case GLFW_GAMEPAD_BUTTON_Y:
                    button = GAMEPAD_BUTTON_RIGHT_FACE_UP;
                    break;
                case GLFW_GAMEPAD_BUTTON_B:
                    button = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
                    break;
                case GLFW_GAMEPAD_BUTTON_A:
                    button = GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
                    break;
                case GLFW_GAMEPAD_BUTTON_X:
                    button = GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
                    break;

                case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                    button = GAMEPAD_BUTTON_LEFT_TRIGGER_1;
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                    button = GAMEPAD_BUTTON_RIGHT_TRIGGER_1;
                    break;

                case GLFW_GAMEPAD_BUTTON_BACK:
                    button = GAMEPAD_BUTTON_MIDDLE_LEFT;
                    break;
                case GLFW_GAMEPAD_BUTTON_GUIDE:
                    button = GAMEPAD_BUTTON_MIDDLE;
                    break;
                case GLFW_GAMEPAD_BUTTON_START:
                    button = GAMEPAD_BUTTON_MIDDLE_RIGHT;
                    break;

                case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                    button = GAMEPAD_BUTTON_LEFT_FACE_UP;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                    button = GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                    button = GAMEPAD_BUTTON_LEFT_FACE_DOWN;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
                    button = GAMEPAD_BUTTON_LEFT_FACE_LEFT;
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
                        CORE.Input.Gamepad.button_state_current[i][button] = 1;
                        CORE.Input.Gamepad.last_button_pressed = button;
                    } else
                        CORE.Input.Gamepad.button_state_current[i][button] = 0;
                }
            }

            // Get current axis state
            const float *axes = state.axes;

            for (int k = 0;
                 (axes != NULL) && (k < GLFW_GAMEPAD_AXIS_LAST + 1) && (k < MAX_GAMEPAD_AXES);
                 k++) {
                CORE.Input.Gamepad.axis_state[i][k] = axes[k];
            }

            // Register buttons for 2nd triggers (because GLFW doesn't count these as buttons
            // but rather axis)
            CORE.Input.Gamepad.button_state_current[i][GAMEPAD_BUTTON_LEFT_TRIGGER_2] =
                (char)(CORE.Input.Gamepad.axis_state[i][GAMEPAD_AXIS_LEFT_TRIGGER] > 0.1);
            CORE.Input.Gamepad.button_state_current[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_2] =
                (char)(CORE.Input.Gamepad.axis_state[i][GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.1);

            CORE.Input.Gamepad.num_available_axes = GLFW_GAMEPAD_AXIS_LAST;
        }
    }

    CORE.Window.was_resized_last_frame = false;

    glfwPollEvents();
}

// Detect if a key has been pressed once
static inline void input_is_key_pressed(int key) {
    return (CORE.Input.Keyboard.key_state_previous[key] == 0) &&
           (CORE.Input.Keyboard.key_state_current[key] == 1);
}

// Detect if a key is being pressed (key held down)
bool input_is_key_down(int key) {
    return CORE.Input.Keyboard.key_state_current[key] == 1;
}

// Detect if a key has been released once
bool input_is_key_released(int key) {
    return (CORE.Input.Keyboard.key_state_previous[key] == 1) &&
           (CORE.Input.Keyboard.key_state_current[key] == 0);
}

// Detect if a key is NOT being pressed (key not held down)
bool input_is_key_up(int key) {
    return CORE.Input.Keyboard.key_state_current[key] == 0;
}

// Get the last key pressed
int input_get_last_key_pressed(void) {
    int value = 0;

    if (CORE.Input.Keyboard.num_keys_pressed > 0) {
        // Get character from the queue head
        value = CORE.Input.Keyboard.key_pressed_queue[0];

        // Shift elements 1 step toward the head.
        for (int i = 0; i < (CORE.Input.Keyboard.num_keys_pressed - 1); i++) {
            CORE.Input.Keyboard.key_pressed_queue[i] = CORE.Input.Keyboard.key_pressed_queue[i + 1];
        }

        // Reset last character in the queue
        CORE.Input.Keyboard.key_pressed_queue[CORE.Input.Keyboard.num_keys_pressed] = 0;
        CORE.Input.Keyboard.num_keys_pressed--;
    }

    return value;
}

// Get the last char pressed
int input_get_last_char_pressed(void) {
    int value = 0;

    if (CORE.Input.Keyboard.num_chars_pressed > 0) {
        // Get character from the queue head
        value = CORE.Input.Keyboard.char_pressed_queue[0];

        // Shift elements 1 step toward the head.
        for (int i = 0; i < (CORE.Input.Keyboard.num_chars_pressed - 1); i++) {
            CORE.Input.Keyboard.char_pressed_queue[i] =
                CORE.Input.Keyboard.char_pressed_queue[i + 1];
        }

        // Reset last character in the queue
        CORE.Input.Keyboard.char_pressed_queue[CORE.Input.Keyboard.num_chars_pressed] = 0;
        CORE.Input.Keyboard.num_chars_pressed--;
    }

    return value;
}

// Return axis movement vector for a gamepad
float input_gamepad_get_axis_movement(int gamepad, int axis) {
    if ((gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.is_ready[gamepad] &&
        (axis < MAX_GAMEPAD_AXES) && (fabsf(CORE.Input.Gamepad.axis_state[gamepad][axis]) > 0.1f)) {
        return CORE.Input.Gamepad.axis_state[gamepad][axis];
    }

    return 0.0f;
}

// Detect if a gamepad button has been pressed once
bool input_gamepad_is_button_pressed(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (CORE.Input.Gamepad.button_state_previous[gamepad][button] == 0) &&
           (CORE.Input.Gamepad.button_state_current[gamepad][button] == 1);
}

// Detect if a gamepad button is being pressed
bool input_gamepad_is_button_down(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (CORE.Input.Gamepad.button_state_current[gamepad][button] == 1);
}

// Detect if a gamepad button has NOT been pressed once
bool input_gamepad_is_button_released(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (CORE.Input.Gamepad.button_state_previous[gamepad][button] == 1) &&
           (CORE.Input.Gamepad.button_state_current[gamepad][button] == 0);
}

// Detect if a mouse button is NOT being pressed
bool input_gamepad_is_button_up(int gamepad, int button) {
    return (gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.is_ready[gamepad] &&
           (button < MAX_GAMEPAD_BUTTONS) &&
           (CORE.Input.Gamepad.button_state_current[gamepad][button] == 0);
}

// Get the last gamepad button pressed
int input_gamepad_get_last_button_pressed(void) {
    return CORE.Input.Gamepad.last_button_pressed;
}

// Set internal gamepad mappings
int input_gamepad_set_mappings(const char *mappings) {
    return glfwUpdateGamepadMappings(mappings);
}

// Detect if a mouse button has been pressed once
bool input_mouse_is_button_pressed(int button) {
    return (CORE.Input.Mouse.button_state_current[button] == 1) &&
           (CORE.Input.Mouse.button_state_previous[button] == 0);
}

// Detect if a mouse button is being pressed
bool input_mouse_is_button_down(int button) {
    return CORE.Input.Mouse.button_state_current[button] == 1;
}

// Detect if a mouse button has been released once
bool input_mouse_is_button_released(int button) {
    return (CORE.Input.Mouse.button_state_current[button] == 0) &&
           (CORE.Input.Mouse.button_state_previous[button] == 1);
}

// Detect if a mouse button is NOT being pressed
bool input_mouse_is_button_up(int button) {
    return !input_mouse_is_button_down(button);
}

// Returns mouse position X
int input_mouse_get_x(void) {
    return (int)((CORE.Input.Mouse.position.x + CORE.Input.Mouse.offset.x) *
                 CORE.Input.Mouse.scale.x);
}

// Returns mouse position Y
int input_mouse_get_y(void) {
    return (int)((CORE.Input.Mouse.position.y + CORE.Input.Mouse.offset.y) *
                 CORE.Input.Mouse.scale.y);
}

// Returns mouse position XY
vec2 input_mouse_get_position(void) {
    return math_vec2(
        (CORE.Input.Mouse.position.x + CORE.Input.Mouse.offset.x) * CORE.Input.Mouse.scale.x,
        (CORE.Input.Mouse.position.y + CORE.Input.Mouse.offset.y) * CORE.Input.Mouse.scale.y);
}

// Set mouse position XY
void input_mouse_set_position(int x, int y) {
    CORE.Input.Mouse.position = math_vec2((float)x, (float)y);
    glfwSetCursorPos(CORE.Window.glfw_window, CORE.Input.Mouse.position.x,
                     CORE.Input.Mouse.position.y);
}

// Set mouse offset
// NOTE: Useful when rendering to different size targets
void input_mouse_set_offset(int offset_x, int offset_y) {
    CORE.Input.Mouse.offset = math_vec2(offset_x, offset_y);
}

// Set mouse scaling
// NOTE: Useful when rendering to different size targets
void input_mouse_set_scale(float scale_x, float scale_y) {
    CORE.Input.Mouse.scale = math_vec2(scale_x, scale_y);
}

// Returns mouse wheel movement Y
float input_mouse_get_wheel_move(void) {
    return CORE.Input.Mouse.wheel_move_previous;
}

// Returns mouse cursor
int input_mouse_get_cursor(void) {
    return CORE.Input.Mouse.cursor;
}

// Set mouse cursor
// NOTE: This is a no-op on platforms other than PLATFORM_DESKTOP
void input_mouse_set_cursor(int cursor) {
    CORE.Input.Mouse.cursor = cursor;
    if (cursor == MOUSE_CURSOR_DEFAULT)
        glfwSetCursor(CORE.Window.glfw_window, NULL);
    else {
        // NOTE: We are relating internal GLFW enum values to our MouseCursor enum values
        glfwSetCursor(CORE.Window.glfw_window, glfwCreateStandardCursor(0x00036000 + cursor));
    }
}

#endif