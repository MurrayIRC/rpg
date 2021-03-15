#ifndef INPUT_H
#define INPUT_H

#include "core.h"

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

#endif