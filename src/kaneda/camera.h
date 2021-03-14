#ifndef CAMERA_H
#define CAMERA_H

#include "kore.h"
#include "math.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from
// window-system specific input methods
typedef enum { FORWARD, BACKWARD, LEFT, RIGHT } CameraMoveDirection;

// Default camera values
static const float CAMERA_DEFAULT_YAW = -90.0f;
static const float CAMERA_DEFAULT_PITCH = 0.0f;
static const float CAMERA_DEFAULT_SPEED = 2.5f;
static const float CAMERA_DEFAULT_SENSITIVITY = 0.1f;
static const float CAMERA_DEFAULT_ZOOM = 45.0f;

typedef struct {
    // camera Attributes
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float move_speed;
    float mouse_sensitivity;
    float zoom;
} Camera;

Camera *camera_create(vec3 pos, vec3 up, float yaw, float pitch);
void camera_update(Camera *cam);
void camera_destroy(Camera *cam);

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
mat4 camera_get_view_matrix(Camera *cam);

// processes input received from any keyboard-like input system. Accepts input parameter in the form
// of camera defined ENUM (to abstract it from windowing systems)
void camera_process_keyboard(Camera *cam, CameraMoveDirection dir, float delta_time);

// processes input received from a mouse input system. Expects the offset value in both the x and y
// direction.
void camera_process_mouse_movement(Camera *cam, float x_offset, float y_offset,
                                   bool constrain_pitch);

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical
// wheel-axis
void camera_process_mouse_scroll(Camera *cam, float y_offset);

#endif
