#include "camera.h"

Camera *camera_create(vec3 pos, vec3 up, float yaw, float pitch) {
    Camera *camera;
    camera = malloc(sizeof(*camera));
    camera->position = pos;
    camera->world_up = up;
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera_update(camera);
    return camera;
}

void camera_update(Camera *cam) {
    // calculate the new Front vector
    vec3 front = math_vec3_zero();
    front.x = cosf(math_deg2rad(cam->yaw)) * cosf(math_deg2rad(cam->pitch));
    front.y = sinf(math_deg2rad(cam->pitch));
    front.z = sinf(math_deg2rad(cam->yaw)) * cosf(math_deg2rad(cam->pitch));
    cam->front = math_vec3_normalize(front);
    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets closer to 0 the more you look up or down
    // which results in slower movement.
    cam->right = math_vec3_normalize(math_vec3_cross(cam->front, cam->world_up));
    cam->up = math_vec3_normalize(math_vec3_cross(cam->right, cam->front));
}

mat4 camera_get_view_matrix(Camera *cam) {
    return math_lookat(cam->position, math_vec3_add(cam->position, cam->front), cam->up);
}

void camera_process_keyboard(Camera *cam, CameraMoveDirection dir, float delta_time) {
    float velocity = cam->move_speed * delta_time;
    if (dir == FORWARD) {
        cam->position =
            math_vec3_add(cam->position, math_vec3_scalar_multiply(cam->front, velocity));
    }
    if (dir == BACKWARD) {
        cam->position =
            math_vec3_subtract(cam->position, math_vec3_scalar_multiply(cam->front, velocity));
    }
    if (dir == LEFT) {
        cam->position =
            math_vec3_subtract(cam->position, math_vec3_scalar_multiply(cam->right, velocity));
    }
    if (dir == RIGHT) {
        cam->position =
            math_vec3_add(cam->position, math_vec3_scalar_multiply(cam->right, velocity));
    }
}

void camera_process_mouse_movement(Camera *cam, float x_offset, float y_offset,
                                   bool constrain_pitch) {
    x_offset *= cam->mouse_sensitivity;
    y_offset *= cam->mouse_sensitivity;

    cam->yaw += x_offset;
    cam->pitch += y_offset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch) {
        if (cam->pitch > 89.0f)
            cam->pitch = 89.0f;
        if (cam->pitch < -89.0f)
            cam->pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    camera_update(cam);
}

void camera_process_mouse_scroll(Camera *cam, float y_offset) {
    cam->zoom -= y_offset;
    if (cam->zoom < 1.0f)
        cam->zoom = 1.0f;
    if (cam->zoom > 45.0f)
        cam->zoom = 45.0f;
}
