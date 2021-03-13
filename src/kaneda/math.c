#include "kore.h"
#include "log.h"
#include <assert.h>
#include <math.h>

float math_sqrt(float number) {
    int i;
    float x, y;
    x = number * 0.5;
    y = number;
    i = *(int *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5 - (x * y * y));
    y = y * (1.5 - (x * y * y));
    return number * y;
}

float math_randf(uint32 index) {
    index = (index << 13) ^ index;
    return (((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) /
            1073741824.0f) *
           0.5f;
}

float math_randnf(uint32 index) {
    index = (index << 13) ^ index;
    return (((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) /
            1073741824.0f) -
           1.0f;
}

uint math_randi(uint32 index) {
    index = (index << 13) ^ index;
    return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

float math_deg2rad(float degrees) {
    return degrees * (PI / 180.0f);
}

float math_rad2deg(float radians) {
    return radians * (180.0f / PI);
}

void math_perspective(float *out_mat, float fov, float aspect, float z_near, float z_far) {
    assert(aspect != 0);
    assert(z_far != z_near);

    float q = 1.0f / (float)tan(math_deg2rad(0.5f * fov));
    float a = q / aspect;
    float b = (z_near + z_far) / (z_near - z_far);
    float c = (2.0f * z_near * z_far) / (z_near - z_far);

    out_mat[0 + 0 * 4] = a;
    out_mat[1 + 1 * 4] = q;
    out_mat[2 + 2 * 4] = b;
    out_mat[2 + 3 * 4] = c;
    out_mat[3 + 2 * 4] = -1.0f;
}

void math_orthographic(float *out_mat, float left, float right, float bottom, float top,
                       float z_near, float z_far) {
    math_matrix_identity(&out_mat);

    // Main diagonal
    out_mat[0 + 0 * 4] = 2.0f / (right - left);
    out_mat[1 + 1 * 4] = 2.0f / (top - bottom);
    out_mat[2 + 2 * 4] = -2.0f / (z_far - z_near);

    // Last column
    out_mat[0 + 3 * 4] = -(right + left) / (right - left);
    out_mat[1 + 3 * 4] = -(top + bottom) / (top - bottom);
    out_mat[2 + 3 * 4] = -(z_far + z_near) / (z_far - z_near);

    // out_mat[0] = 2.0f / (fov - (-fov));
    // out_mat[5] = 2.0f / (fov - (-fov));
    // out_mat[10] = -2.0f / (z_far - z_near);
    // out_mat[3] = -(fov + (-fov)) / (fov - (-fov));
    // out_mat[7] = -(fov + (-fov)) / (fov - (-fov));
    // out_mat[11] = -(z_far + z_near) / (z_far - z_near);
}

void math_lookat(float *out_mat, float *vec_pos, float *vec_target, float *vec_up) {
    float f[3] = {vec_target[0] - vec_pos[0], vec_target[1] - vec_pos[1],
                  vec_target[2] - vec_pos[2]};
    math_normalize3d(&f);

    log_info("Hey!\n");

    float s[3];
    math_cross3d(&s, f, vec_up);
    math_normalize3d(&s);

    float u[3];
    math_cross3d(&u, s, f);

    math_matrix_identity(&out_mat);
    out_mat[0 * 4 + 0] = s[0];
    out_mat[1 * 4 + 0] = s[1];
    out_mat[2 * 4 + 0] = s[2];

    out_mat[0 * 4 + 1] = u[0];
    out_mat[1 * 4 + 1] = u[1];
    out_mat[2 * 4 + 1] = u[2];

    out_mat[0 * 4 + 2] = -f[0];
    out_mat[1 * 4 + 2] = -f[1];
    out_mat[2 * 4 + 2] = -f[2];

    out_mat[3 * 4 + 0] = -math_dot3d(s, vec_pos);
    out_mat[3 * 4 + 1] = -math_dot3d(u, vec_pos);
    out_mat[3 * 4 + 2] = math_dot3d(f, vec_pos);
}

float math_length2d(float *vec) {
    return math_sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}

float math_length3d(float *vec) {
    return math_sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

float math_distance2d(float *a, float *b) {
    float tmp[2];
    tmp[0] = a[0] - b[0];
    tmp[1] = a[1] - b[1];
    return math_sqrt(tmp[0] * tmp[0] + tmp[1] * tmp[1]);
}

float math_distance3d(float *a, float *b) {
    float tmp[3];
    tmp[0] = a[0] - b[0];
    tmp[1] = a[1] - b[1];
    tmp[2] = a[2] - b[2];
    return math_sqrt(tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2]);
}

float math_dot2d(float *a, float *b) {
    return a[0] * b[0] + a[1] * b[1];
}

float math_dot3d(float *a, float *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void math_cross2d(float *output, float *a, float *b) {
    output[0] = a[1];
    output[1] = -a[0];
}

void math_cross3d(float *output, float *a, float *b) {
    output[0] = a[1] * b[2] - a[2] * b[1];
    output[1] = a[2] * b[0] - a[0] * b[2];
    output[2] = a[0] * b[1] - a[1] * b[0];
}

void math_normalize2d(float *vec) {
    float f = math_sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
    vec[0] /= f;
    vec[1] /= f;
}

void math_normalize3d(float *vec) {
    float f = math_sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    vec[0] /= f;
    vec[1] /= f;
    vec[2] /= f;
}

void math_normalize4d(float *vec) {
    float f = math_sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]);
    vec[0] /= f;
    vec[1] /= f;
    vec[2] /= f;
    vec[3] /= f;
}

void math_normal2d(float *output, float *a, float *b) {
    output[0] = a[1] - b[1];
    output[1] = b[0] - a[0];
    float f = math_sqrt(output[0] * output[0] + output[1] * output[1]);
    output[0] /= f;
    output[1] /= f;
}

void math_normal3d(float *output, float *a, float *b, float *c) {
    float a2[3], b2[3], f;
    a2[0] = a[0] - c[0];
    a2[1] = a[1] - c[1];
    a2[2] = a[2] - c[2];
    b2[0] = b[0] - c[0];
    b2[1] = b[1] - c[1];
    b2[2] = b[2] - c[2];
    output[0] = a2[1] * b2[2] - a2[2] * b2[1];
    output[1] = a2[2] * b2[0] - a2[0] * b2[2];
    output[2] = a2[0] * b2[1] - a2[1] * b2[0];
    f = math_sqrt(output[0] * output[0] + output[1] * output[1] + output[2] * output[2]);
    output[0] /= f;
    output[1] /= f;
    output[2] /= f;
}

void math_reflect2d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1];
    output[0] = pos[0] - normal[0] * 2.0 * f;
    output[1] = pos[1] - normal[1] * 2.0 * f;
}

void math_reflect3d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1] + pos[2] * normal[2];
    output[0] = pos[0] - normal[0] * 2.0 * f;
    output[1] = pos[1] - normal[1] * 2.0 * f;
    output[2] = pos[2] - normal[2] * 2.0 * f;
}

void math_flatten2d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1];
    output[0] = pos[0] - normal[0] * f;
    output[1] = pos[1] - normal[1] * f;
}

void math_flatten3d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1] + pos[2] * normal[2];
    output[0] = pos[0] - normal[0] * f;
    output[1] = pos[1] - normal[1] * f;
    output[2] = pos[2] - normal[2] * f;
}

void math_project2d(float *output, float *plane_pos, float *normal, float *pos, float *vector) {
    float f = normal[0] * (plane_pos[0] - pos[0]) + normal[1] * (plane_pos[1] - pos[1]);
    f /= normal[0] * vector[0] + normal[1] * vector[1];
    output[0] = pos[0] + vector[0] * f;
    output[1] = pos[1] + vector[1] * f;
}

void math_project3d(float *output, float *plane_pos, float *normal, float *pos, float *vector) {
    float f = normal[0] * (plane_pos[0] - pos[0]) + normal[1] * (plane_pos[1] - pos[1]) +
              normal[2] * (plane_pos[2] - pos[2]);
    f /= normal[0] * vector[0] + normal[1] * vector[1] + normal[2] * vector[2];
    output[0] = pos[0] + vector[0] * f;
    output[1] = pos[1] + vector[1] * f;
    output[2] = pos[2] + vector[2] * f;
}

void math_intersect2d(float *output, float *line_a0, float *line_a1, float *line_b0,
                      float *line_b1) {
    output[0] = (line_a0[0] * line_a1[1] - line_a0[1] * line_a1[0]) * (line_b0[0] - line_b1[0]) -
                (line_b0[0] * line_b1[1] - line_b0[1] * line_b1[0]) * (line_a0[0] - line_a1[0]);
    output[0] /= (line_a0[0] - line_a1[0]) * (line_b0[1] - line_b1[1]) -
                 (line_a0[1] - line_a1[1]) * (line_b0[0] - line_b1[0]);
    output[1] = (line_a0[0] * line_a1[1] - line_a0[1] * line_a1[0]) * (line_b0[1] - line_b1[1]) -
                (line_b0[0] * line_b1[1] - line_b0[1] * line_b1[0]) * (line_a0[1] - line_a1[1]);
    output[1] /= (line_a0[0] - line_a1[0]) * (line_b0[1] - line_b1[1]) -
                 (line_a0[1] - line_a1[1]) * (line_b0[0] - line_b1[0]);
}

#define sqrt_step(shift)                                                                           \
    if ((0x40000000l >> shift) + root <= value) {                                                  \
        value -= (0x40000000l >> shift) + root;                                                    \
        root = (root >> 1) | (0x40000000l >> shift);                                               \
    } else {                                                                                       \
        root = root >> 1;                                                                          \
    }

long math_sqrti(long value) {
    long root = 0;
    sqrt_step(0);
    sqrt_step(2);
    sqrt_step(4);
    sqrt_step(6);
    sqrt_step(8);
    sqrt_step(10);
    sqrt_step(12);
    sqrt_step(14);
    sqrt_step(16);
    sqrt_step(18);
    sqrt_step(20);
    sqrt_step(22);
    sqrt_step(24);
    sqrt_step(26);
    sqrt_step(28);
    sqrt_step(30);
    if (root < value) {
        ++root;
    }
    return root;
}

bool math_normalize2di(int *point, int fixed_point_multiplier) {
    int length = math_sqrti(point[0] * point[0] + point[1] * point[1]);
    if (length != 0) {
        point[0] = (point[0] * fixed_point_multiplier) / length;
        point[1] = (point[1] * fixed_point_multiplier) / length;
        return 1;
    }
    return 0;
}

bool math_normalize3di(int *point, int fixed_point_multiplier) {
    int length = math_sqrti(point[0] * point[0] + point[1] * point[1] + point[2] * point[2]);
    if (length != 0) {
        point[0] = (point[0] * fixed_point_multiplier) / length;
        point[1] = (point[1] * fixed_point_multiplier) / length;
        point[2] = (point[2] * fixed_point_multiplier) / length;
        return 1;
    }
    return 0;
}

void math_intersect2di(int *output, int *line_a0, int *line_a1, int *line_b0, int *line_b1) {
    int64 i, tmp, line64_a0[2], line64_a1[2], line64_b0[2], line64_b1[2];
    line64_a0[0] = (int64)line_a0[0];
    line64_a0[1] = (int64)line_a0[1];
    line64_a1[0] = (int64)line_a1[0];
    line64_a1[1] = (int64)line_a1[1];
    line64_b0[0] = (int64)line_b0[0];
    line64_b0[1] = (int64)line_b0[1];
    line64_b1[0] = (int64)line_b1[0];
    line64_b1[1] = (int64)line_b1[1];
    i = (line64_a0[0] - line64_a1[0]) * (line64_b0[1] - line64_b1[1]) -
        (line64_a0[1] - line64_a1[1]) * (line64_b0[0] - line64_b1[0]);
    if (i == 0) {
        output[0] = (line_a0[0] + line_a1[0]) / 2;
        output[1] = (line_a0[1] + line_a1[1]) / 2;
        return;
    }
    tmp =
        (line64_a0[0] * line64_a1[1] - line64_a0[1] * line64_a1[0]) *
            (line64_b0[0] - line64_b1[0]) -
        (line64_b0[0] * line64_b1[1] - line64_b0[1] * line64_b1[0]) * (line64_a0[0] - line64_a1[0]);
    tmp /= i;
    output[0] = (int)tmp;
    i = (line64_a0[0] - line64_a1[0]) * (line64_b0[1] - line64_b1[1]) -
        (line64_a0[1] - line64_a1[1]) * (line64_b0[0] - line64_b1[0]);
    if (i == 0) {
        output[0] = (line_a0[0] + line_a1[0]) / 2;
        output[1] = (line_a0[1] + line_a1[1]) / 2;
        return;
    }
    tmp =
        (line64_a0[0] * line64_a1[1] - line64_a0[1] * line64_a1[0]) *
            (line64_b0[1] - line64_b1[1]) -
        (line64_b0[0] * line64_b1[1] - line64_b0[1] * line64_b1[0]) * (line64_a0[1] - line64_a1[1]);
    tmp /= i;
    output[1] = (int)tmp;
}

void math_matrix_identity(float *matrix) {
    matrix[0] = 1;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;
    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = 1;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

/*
0,  1,  2,  3,
4,  5,  6,  7,
8,  9,  10, 11,
12, 13, 14, 15
*/

/*
0, 4, 8,  12
1, 5, 9,  13
2, 6, 10, 14
3, 7, 11, 15
*/
void math_translate(float *matrix, const float x, const float y, const float z) {
    math_matrix_identity(matrix);
    matrix[0 + 4 * 3] = x;
    matrix[1 + 4 * 3] = y;
    matrix[2 + 4 * 3] = z;
}

void math_scale(float *matrix, const float x, const float y, const float z) {
    math_matrix_identity(matrix);
    matrix[0 + 0 * 4] = x;
    matrix[1 + 1 * 4] = y;
    matrix[2 + 2 * 4] = z;
}

void math_rotate(float *matrix, const float angle, const float axis_x, const float axis_y,
                 const float axis_z) {
    float a = angle;
    float c = (float)cos(a);
    float s = (float)sin(a);

    float n_axis[3] = {axis_x, axis_y, axis_z};
    math_normalize3d(&n_axis);

    float x = n_axis[0];
    float y = n_axis[1];
    float z = n_axis[2];

    // First column
    matrix[0 + 0 * 4] = x * x * (1 - c) + c;
    matrix[1 + 0 * 4] = x * y * (1 - c) + z * s;
    matrix[2 + 0 * 4] = x * z * (1 - c) - y * s;

    // Second column
    matrix[0 + 1 * 4] = x * y * (1 - c) - z * s;
    matrix[1 + 1 * 4] = y * y * (1 - c) + c;
    matrix[2 + 1 * 4] = y * z * (1 - c) + x * s;

    // Third column
    matrix[0 + 2 * 4] = x * z * (1 - c) + y * s;
    matrix[1 + 2 * 4] = y * z * (1 - c) - x * s;
    matrix[2 + 2 * 4] = z * z * (1 - c) + c;
}

void math_transform3d(float *output, const float *matrix, const float x, const float y,
                      const float z) {
    output[0] = (matrix[0] * x) + (matrix[4] * y) + (matrix[8] * z) + matrix[12];
    output[1] = (matrix[1] * x) + (matrix[5] * y) + (matrix[9] * z) + matrix[13];
    output[2] = (matrix[2] * x) + (matrix[6] * y) + (matrix[10] * z) + matrix[14];
}

void math_transform_inv3d(float *out, const float *matrix, float x, float y, float z) {
    x -= matrix[12];
    y -= matrix[13];
    z -= matrix[14];
    out[0] = (matrix[0] * x) + (matrix[1] * y) + (matrix[2] * z);
    out[1] = (matrix[4] * x) + (matrix[5] * y) + (matrix[6] * z);
    out[2] = (matrix[8] * x) + (matrix[9] * y) + (matrix[10] * z);
}

void math_transform4d(float *output, const float *matrix, const float x, const float y,
                      const float z, const double w) {
    output[0] = (matrix[0] * x) + (matrix[4] * y) + (matrix[8] * z) + (matrix[12] * w);
    output[1] = (matrix[1] * x) + (matrix[5] * y) + (matrix[9] * z) + (matrix[13] * w);
    output[2] = (matrix[2] * x) + (matrix[6] * y) + (matrix[10] * z) + (matrix[14] * w);
    output[3] = (matrix[3] * x) + (matrix[7] * y) + (matrix[11] * z) + (matrix[15] * w);
}

void math_matrix_multiply(float *output, const float *a, const float *b) {
    output[0] = (b[0] * a[0]) + (b[1] * a[4]) + (b[2] * a[8]) + (b[3] * a[12]);
    output[4] = (b[4] * a[0]) + (b[5] * a[4]) + (b[6] * a[8]) + (b[7] * a[12]);
    output[8] = (b[8] * a[0]) + (b[9] * a[4]) + (b[10] * a[8]) + (b[11] * a[12]);
    output[12] = (b[12] * a[0]) + (b[13] * a[4]) + (b[14] * a[8]) + (b[15] * a[12]);

    output[1] = (b[0] * a[1]) + (b[1] * a[5]) + (b[2] * a[9]) + (b[3] * a[13]);
    output[5] = (b[4] * a[1]) + (b[5] * a[5]) + (b[6] * a[9]) + (b[7] * a[13]);
    output[9] = (b[8] * a[1]) + (b[9] * a[5]) + (b[10] * a[9]) + (b[11] * a[13]);
    output[13] = (b[12] * a[1]) + (b[13] * a[5]) + (b[14] * a[9]) + (b[15] * a[13]);

    output[2] = (b[0] * a[2]) + (b[1] * a[6]) + (b[2] * a[10]) + (b[3] * a[14]);
    output[6] = (b[4] * a[2]) + (b[5] * a[6]) + (b[6] * a[10]) + (b[7] * a[14]);
    output[10] = (b[8] * a[2]) + (b[9] * a[6]) + (b[10] * a[10]) + (b[11] * a[14]);
    output[14] = (b[12] * a[2]) + (b[13] * a[6]) + (b[14] * a[10]) + (b[15] * a[14]);

    output[3] = (b[0] * a[3]) + (b[1] * a[7]) + (b[2] * a[11]) + (b[3] * a[15]);
    output[7] = (b[4] * a[3]) + (b[5] * a[7]) + (b[6] * a[11]) + (b[7] * a[15]);
    output[11] = (b[8] * a[3]) + (b[9] * a[7]) + (b[10] * a[11]) + (b[11] * a[15]);
    output[15] = (b[12] * a[3]) + (b[13] * a[7]) + (b[14] * a[11]) + (b[15] * a[15]);
}

void math_matrix_reverse4x4(float *output, const float *matrix) {
    output[0] = matrix[5] * matrix[10] * matrix[15] + matrix[6] * matrix[11] * matrix[13] +
                matrix[7] * matrix[9] * matrix[14] - matrix[5] * matrix[11] * matrix[14] -
                matrix[6] * matrix[9] * matrix[15] - matrix[7] * matrix[10] * matrix[13];
    output[1] = matrix[1] * matrix[11] * matrix[14] + matrix[2] * matrix[9] * matrix[15] +
                matrix[3] * matrix[10] * matrix[13] - matrix[1] * matrix[10] * matrix[15] -
                matrix[2] * matrix[11] * matrix[13] - matrix[3] * matrix[9] * matrix[14];
    output[2] = matrix[1] * matrix[6] * matrix[15] + matrix[2] * matrix[7] * matrix[13] +
                matrix[3] * matrix[5] * matrix[14] - matrix[1] * matrix[7] * matrix[14] -
                matrix[2] * matrix[5] * matrix[15] - matrix[3] * matrix[6] * matrix[13];
    output[3] = matrix[1] * matrix[7] * matrix[10] + matrix[2] * matrix[5] * matrix[11] +
                matrix[3] * matrix[6] * matrix[9] - matrix[1] * matrix[6] * matrix[11] -
                matrix[2] * matrix[7] * matrix[9] - matrix[3] * matrix[5] * matrix[10];
    output[4] = matrix[4] * matrix[11] * matrix[14] + matrix[6] * matrix[8] * matrix[15] +
                matrix[7] * matrix[10] * matrix[12] - matrix[4] * matrix[10] * matrix[15] -
                matrix[6] * matrix[11] * matrix[12] - matrix[7] * matrix[8] * matrix[14];
    output[5] = matrix[0] * matrix[10] * matrix[15] + matrix[2] * matrix[11] * matrix[12] +
                matrix[3] * matrix[8] * matrix[14] - matrix[0] * matrix[11] * matrix[14] -
                matrix[2] * matrix[8] * matrix[15] - matrix[3] * matrix[10] * matrix[12];
    output[6] = matrix[0] * matrix[7] * matrix[14] + matrix[2] * matrix[4] * matrix[15] +
                matrix[3] * matrix[6] * matrix[12] - matrix[0] * matrix[6] * matrix[15] -
                matrix[2] * matrix[7] * matrix[12] - matrix[3] * matrix[4] * matrix[14];
    output[7] = matrix[0] * matrix[6] * matrix[11] + matrix[2] * matrix[7] * matrix[8] +
                matrix[3] * matrix[4] * matrix[10] - matrix[0] * matrix[7] * matrix[10] -
                matrix[2] * matrix[4] * matrix[11] - matrix[3] * matrix[6] * matrix[8];
    output[8] = matrix[4] * matrix[9] * matrix[15] + matrix[5] * matrix[11] * matrix[12] +
                matrix[7] * matrix[8] * matrix[13] - matrix[4] * matrix[11] * matrix[13] -
                matrix[5] * matrix[8] * matrix[15] - matrix[7] * matrix[9] * matrix[12];
    output[9] = matrix[0] * matrix[11] * matrix[13] + matrix[1] * matrix[8] * matrix[15] +
                matrix[3] * matrix[9] * matrix[12] - matrix[0] * matrix[9] * matrix[15] -
                matrix[1] * matrix[11] * matrix[12] - matrix[3] * matrix[8] * matrix[13];
    output[10] = matrix[0] * matrix[5] * matrix[15] + matrix[1] * matrix[7] * matrix[12] +
                 matrix[3] * matrix[4] * matrix[13] - matrix[0] * matrix[7] * matrix[13] -
                 matrix[1] * matrix[4] * matrix[15] - matrix[3] * matrix[5] * matrix[12];
    output[11] = matrix[0] * matrix[7] * matrix[9] + matrix[1] * matrix[4] * matrix[11] +
                 matrix[3] * matrix[5] * matrix[8] - matrix[0] * matrix[5] * matrix[11] -
                 matrix[1] * matrix[7] * matrix[8] - matrix[3] * matrix[4] * matrix[9];
    output[12] = matrix[4] * matrix[10] * matrix[13] + matrix[5] * matrix[8] * matrix[14] +
                 matrix[6] * matrix[9] * matrix[12] - matrix[4] * matrix[9] * matrix[14] -
                 matrix[5] * matrix[10] * matrix[12] - matrix[6] * matrix[8] * matrix[13];
    output[13] = matrix[0] * matrix[9] * matrix[14] + matrix[1] * matrix[10] * matrix[12] +
                 matrix[2] * matrix[8] * matrix[13] - matrix[0] * matrix[10] * matrix[13] -
                 matrix[1] * matrix[8] * matrix[14] - matrix[2] * matrix[9] * matrix[12];
    output[14] = matrix[0] * matrix[6] * matrix[13] + matrix[1] * matrix[4] * matrix[14] +
                 matrix[2] * matrix[5] * matrix[12] - matrix[0] * matrix[5] * matrix[14] -
                 matrix[1] * matrix[6] * matrix[12] - matrix[2] * matrix[4] * matrix[13];
    output[15] = matrix[0] * matrix[5] * matrix[10] + matrix[1] * matrix[6] * matrix[8] +
                 matrix[2] * matrix[4] * matrix[9] - matrix[0] * matrix[6] * matrix[9] -
                 matrix[1] * matrix[4] * matrix[10] - matrix[2] * matrix[5] * matrix[8];
}

void math_quaternion_to_matrix(float *matrix, float x, float y, float z, float w) {
    float xx, xy, xz, xw, yy, yz, yw, zz, zw;
    xx = sqrt(x * x + y * y + z * z + w * w);
    x /= xx;
    y /= xx;
    z /= xx;
    w /= xx;
    xx = x * x;
    xy = x * y;
    xz = x * z;
    xw = x * w;
    yy = y * y;
    yz = y * z;
    yw = y * w;
    zz = z * z;
    zw = z * w;
    matrix[0] = 1 - 2 * (yy + zz);
    matrix[1] = 2 * (xy - zw);
    matrix[2] = 2 * (xz + yw);
    matrix[4] = 2 * (xy + zw);
    matrix[5] = 1 - 2 * (xx + zz);
    matrix[6] = 2 * (yz - xw);
    matrix[8] = 2 * (xz - yw);
    matrix[9] = 2 * (yz + xw);
    matrix[10] = 1 - 2 * (xx + yy);
    matrix[3] = matrix[7] = matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0;
    matrix[15] = 1;
}

void math_matrix_to_quaternion(float *quaternion, float *matrix) {
    float trace, s;
    trace = matrix[0 * 4 + 0] + matrix[1 * 4 + 1] + matrix[2 * 4 + 2];
    if (trace > 0.0f) {
        s = 0.5f / sqrt(trace + 1.0f);
        quaternion[3] = 0.25f / s;
        quaternion[0] = (matrix[2 * 4 + 1] - matrix[1 * 4 + 2]) * s;
        quaternion[1] = (matrix[0 * 4 + 2] - matrix[2 * 4 + 0]) * s;
        quaternion[2] = (matrix[1 * 4 + 0] - matrix[0 * 4 + 1]) * s;
    } else {
        if (matrix[0 * 4 + 0] > matrix[1 * 4 + 1] && matrix[0 * 4 + 0] > matrix[2 * 4 + 2]) {
            s = 2.0f * sqrt(1.0f + matrix[0 * 4 + 0] - matrix[1 * 4 + 1] - matrix[2 * 4 + 2]);
            quaternion[3] = (matrix[2 * 4 + 1] - matrix[1 * 4 + 2]) / s;
            quaternion[0] = 0.25f * s;
            quaternion[1] = (matrix[0 * 4 + 1] + matrix[1 * 4 + 0]) / s;
            quaternion[2] = (matrix[0 * 4 + 2] + matrix[2 * 4 + 0]) / s;
        } else if (matrix[1 * 4 + 1] > matrix[2 * 4 + 2]) {
            s = 2.0f * sqrt(1.0f + matrix[1 * 4 + 1] - matrix[0 * 4 + 0] - matrix[2 * 4 + 2]);
            quaternion[3] = (matrix[0 * 4 + 2] - matrix[2 * 4 + 0]) / s;
            quaternion[0] = (matrix[0 * 4 + 1] + matrix[1 * 4 + 0]) / s;
            quaternion[1] = 0.25f * s;
            quaternion[2] = (matrix[1 * 4 + 2] + matrix[2 * 4 + 1]) / s;
        } else {
            s = 2.0f * sqrt(1.0f + matrix[2 * 4 + 2] - matrix[0 * 4 + 0] - matrix[1 * 4 + 1]);
            quaternion[3] = (matrix[1 * 4 + 0] - matrix[0 * 4 + 1]) / s;
            quaternion[0] = (matrix[0 * 4 + 2] + matrix[2 * 4 + 0]) / s;
            quaternion[1] = (matrix[1 * 4 + 2] + matrix[2 * 4 + 1]) / s;
            quaternion[2] = 0.25f * s;
        }
    }
    s = sqrt(quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1] +
             quaternion[2] * quaternion[2] + quaternion[3] * quaternion[3]);
    quaternion[0] /= s;
    quaternion[1] /= s;
    quaternion[2] /= s;
    quaternion[3] /= s;
}

void math_matrix_to_pos_quaternion_scale(float *matrix, float *pos, float *quaternion,
                                         float *scale) {
    math_matrix_to_quaternion(quaternion, matrix);
    pos[0] = matrix[12];
    pos[1] = matrix[13];
    pos[2] = matrix[14];
    scale[0] = math_length3d(matrix);
    scale[1] = math_length3d(&matrix[4]);
    scale[2] = math_length3d(&matrix[8]);
}

void math_pos_quaternion_scale_to_matrix(float *pos, float *quaternion, float *scale,
                                         float *matrix) {
    math_quaternion_to_matrix(matrix, quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
    matrix[12] = pos[0];
    matrix[13] = pos[1];
    matrix[14] = pos[2];
    matrix[0] *= scale[0];
    matrix[1] *= scale[0];
    matrix[2] *= scale[0];
    matrix[4] *= scale[1];
    matrix[5] *= scale[1];
    matrix[6] *= scale[1];
    matrix[8] *= scale[2];
    matrix[9] *= scale[2];
    matrix[10] *= scale[2];
}

void math_matrixxy(float *matrix, const float *origin, const float *point_a, const float *point_b) {
    float r, a[3], b[3];
    if (origin != NULL) {
        a[0] = point_a[0] - origin[0];
        a[1] = point_a[1] - origin[1];
        a[2] = point_a[2] - origin[2];
        matrix[12] = origin[0];
        matrix[13] = origin[1];
        matrix[14] = origin[2];
        b[0] = point_b[0] - origin[0];
        b[1] = point_b[1] - origin[1];
        b[2] = point_b[2] - origin[2];
    } else {
        a[0] = point_a[0];
        a[1] = point_a[1];
        a[2] = point_a[2];
        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        b[0] = point_b[0];
        b[1] = point_b[1];
        b[2] = point_b[2];
    }
    r = math_sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    matrix[0] = a[0] / r;
    matrix[1] = a[1] / r;
    matrix[2] = a[2] / r;
    matrix[8] = matrix[1] * b[2] - matrix[2] * b[1];
    matrix[9] = matrix[2] * b[0] - matrix[0] * b[2];
    matrix[10] = matrix[0] * b[1] - matrix[1] * b[0];
    r = math_sqrt(matrix[8] * matrix[8] + matrix[9] * matrix[9] + matrix[10] * matrix[10]);
    matrix[8] = matrix[8] / r;
    matrix[9] = matrix[9] / r;
    matrix[10] = matrix[10] / r;
    matrix[4] = matrix[2] * matrix[9] - matrix[1] * matrix[10];
    matrix[5] = matrix[0] * matrix[10] - matrix[2] * matrix[8];
    matrix[6] = matrix[1] * matrix[8] - matrix[0] * matrix[9];
    r = math_sqrt(matrix[4] * matrix[4] + matrix[5] * matrix[5] + matrix[6] * matrix[6]);
    matrix[4] = matrix[4] / r;
    matrix[5] = matrix[5] / r;
    matrix[6] = matrix[6] / r;
    matrix[3] = 0;
    matrix[7] = 0;
    matrix[11] = 0;
    matrix[15] = 1;
}

void math_matrixxz(float *matrix, const float *origin, const float *point_a, const float *point_b) {
    float r, a[3], b[3];
    if (origin != NULL) {
        a[0] = point_a[0] - origin[0];
        a[1] = point_a[1] - origin[1];
        a[2] = point_a[2] - origin[2];
        matrix[12] = origin[0];
        matrix[13] = origin[1];
        matrix[14] = origin[2];
        b[0] = point_b[0] - origin[0];
        b[1] = point_b[1] - origin[1];
        b[2] = point_b[2] - origin[2];
    } else {
        a[0] = point_a[0];
        a[1] = point_a[1];
        a[2] = point_a[2];
        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        b[0] = point_b[0];
        b[1] = point_b[1];
        b[2] = point_b[2];
    }
    r = math_sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    matrix[0] = a[0] / r;
    matrix[1] = a[1] / r;
    matrix[2] = a[2] / r;
    matrix[4] = matrix[2] * b[1] - matrix[1] * b[2];
    matrix[5] = matrix[0] * b[2] - matrix[2] * b[0];
    matrix[6] = matrix[1] * b[0] - matrix[0] * b[1];
    r = math_sqrt(matrix[4] * matrix[4] + matrix[5] * matrix[5] + matrix[6] * matrix[6]);
    matrix[4] = matrix[4] / r;
    matrix[5] = matrix[5] / r;
    matrix[6] = matrix[6] / r;
    matrix[8] = matrix[1] * matrix[6] - matrix[2] * matrix[5];
    matrix[9] = matrix[2] * matrix[4] - matrix[0] * matrix[6];
    matrix[10] = matrix[0] * matrix[5] - matrix[1] * matrix[4];
    r = math_sqrt(matrix[8] * matrix[8] + matrix[9] * matrix[9] + matrix[10] * matrix[10]);
    matrix[8] = matrix[8] / r;
    matrix[9] = matrix[9] / r;
    matrix[10] = matrix[10] / r;
    matrix[3] = 0;
    matrix[7] = 0;
    matrix[11] = 0;
    matrix[15] = 1;
}

void math_matrixyx(float *matrix, const float *origin, const float *point_a, const float *point_b) {
    float r, a[3], b[3];
    if (origin != NULL) {
        a[0] = point_a[0] - origin[0];
        a[1] = point_a[1] - origin[1];
        a[2] = point_a[2] - origin[2];
        matrix[12] = origin[0];
        matrix[13] = origin[1];
        matrix[14] = origin[2];
        b[0] = point_b[0] - origin[0];
        b[1] = point_b[1] - origin[1];
        b[2] = point_b[2] - origin[2];
    } else {
        a[0] = point_a[0];
        a[1] = point_a[1];
        a[2] = point_a[2];
        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        b[0] = point_b[0];
        b[1] = point_b[1];
        b[2] = point_b[2];
    }
    r = math_sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    matrix[4] = a[0] / r;
    matrix[5] = a[1] / r;
    matrix[6] = a[2] / r;
    matrix[8] = matrix[6] * b[1] - matrix[5] * b[2];
    matrix[9] = matrix[4] * b[2] - matrix[6] * b[0];
    matrix[10] = matrix[5] * b[0] - matrix[4] * b[1];
    r = math_sqrt(matrix[8] * matrix[8] + matrix[9] * matrix[9] + matrix[10] * matrix[10]);
    matrix[8] = matrix[8] / r;
    matrix[9] = matrix[9] / r;
    matrix[10] = matrix[10] / r;
    matrix[0] = matrix[5] * matrix[10] - matrix[6] * matrix[9];
    matrix[1] = matrix[6] * matrix[8] - matrix[4] * matrix[10];
    matrix[2] = matrix[4] * matrix[9] - matrix[5] * matrix[8];
    r = math_sqrt(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]);
    matrix[0] = matrix[0] / r;
    matrix[1] = matrix[1] / r;
    matrix[2] = matrix[2] / r;
    matrix[3] = 0;
    matrix[7] = 0;
    matrix[11] = 0;
    matrix[15] = 1;
}

void math_matrixyz(float *matrix, const float *origin, const float *point_a, const float *point_b) {
    float r, a[3], b[3];
    if (origin != NULL) {
        a[0] = point_a[0] - origin[0];
        a[1] = point_a[1] - origin[1];
        a[2] = point_a[2] - origin[2];
        matrix[12] = origin[0];
        matrix[13] = origin[1];
        matrix[14] = origin[2];
        b[0] = point_b[0] - origin[0];
        b[1] = point_b[1] - origin[1];
        b[2] = point_b[2] - origin[2];
    } else {
        a[0] = point_a[0];
        a[1] = point_a[1];
        a[2] = point_a[2];
        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        b[0] = point_b[0];
        b[1] = point_b[1];
        b[2] = point_b[2];
    }
    r = math_sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    matrix[4] = a[0] / r;
    matrix[5] = a[1] / r;
    matrix[6] = a[2] / r;
    matrix[0] = matrix[5] * b[2] - matrix[6] * b[1];
    matrix[1] = matrix[6] * b[0] - matrix[4] * b[2];
    matrix[2] = matrix[4] * b[1] - matrix[5] * b[0];
    r = math_sqrt(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]);
    matrix[0] = matrix[0] / r;
    matrix[1] = matrix[1] / r;
    matrix[2] = matrix[2] / r;
    matrix[8] = matrix[6] * matrix[1] - matrix[5] * matrix[2];
    matrix[9] = matrix[4] * matrix[2] - matrix[6] * matrix[0];
    matrix[10] = matrix[5] * matrix[0] - matrix[4] * matrix[1];
    r = math_sqrt(matrix[8] * matrix[8] + matrix[9] * matrix[9] + matrix[10] * matrix[10]);
    matrix[8] = matrix[8] / r;
    matrix[9] = matrix[9] / r;
    matrix[10] = matrix[10] / r;
    matrix[3] = 0;
    matrix[7] = 0;
    matrix[11] = 0;
    matrix[15] = 1;
}

void math_matrixzx(float *matrix, const float *origin, const float *point_a, const float *point_b) {
    float r, a[3], b[3];
    if (origin != NULL) {
        a[0] = point_a[0] - origin[0];
        a[1] = point_a[1] - origin[1];
        a[2] = point_a[2] - origin[2];
        matrix[12] = origin[0];
        matrix[13] = origin[1];
        matrix[14] = origin[2];
        b[0] = point_b[0] - origin[0];
        b[1] = point_b[1] - origin[1];
        b[2] = point_b[2] - origin[2];
    } else {
        a[0] = point_a[0];
        a[1] = point_a[1];
        a[2] = point_a[2];
        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        b[0] = point_b[0];
        b[1] = point_b[1];
        b[2] = point_b[2];
    }
    r = math_sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    matrix[8] = a[0] / r;
    matrix[9] = a[1] / r;
    matrix[10] = a[2] / r;
    matrix[4] = matrix[9] * b[2] - matrix[10] * b[1];
    matrix[5] = matrix[10] * b[0] - matrix[8] * b[2];
    matrix[6] = matrix[8] * b[1] - matrix[9] * b[0];
    r = math_sqrt(matrix[4] * matrix[4] + matrix[5] * matrix[5] + matrix[6] * matrix[6]);
    matrix[4] = matrix[4] / r;
    matrix[5] = matrix[5] / r;
    matrix[6] = matrix[6] / r;
    matrix[0] = matrix[10] * matrix[5] - matrix[9] * matrix[6];
    matrix[1] = matrix[8] * matrix[6] - matrix[10] * matrix[4];
    matrix[2] = matrix[9] * matrix[4] - matrix[8] * matrix[5];
    r = math_sqrt(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]);
    matrix[0] = matrix[0] / r;
    matrix[1] = matrix[1] / r;
    matrix[2] = matrix[2] / r;
    matrix[3] = 0;
    matrix[7] = 0;
    matrix[11] = 0;
    matrix[15] = 1;
}

void math_matrixzy(float *matrix, const float *origin, const float *point_a, const float *point_b) {
    float r, a[3], b[3];
    if (origin != NULL) {
        a[0] = point_a[0] - origin[0];
        a[1] = point_a[1] - origin[1];
        a[2] = point_a[2] - origin[2];
        matrix[12] = origin[0];
        matrix[13] = origin[1];
        matrix[14] = origin[2];
        b[0] = point_b[0] - origin[0];
        b[1] = point_b[1] - origin[1];
        b[2] = point_b[2] - origin[2];
    } else {
        a[0] = point_a[0];
        a[1] = point_a[1];
        a[2] = point_a[2];
        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        b[0] = point_b[0];
        b[1] = point_b[1];
        b[2] = point_b[2];
    }
    r = math_sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    matrix[8] = a[0] / r;
    matrix[9] = a[1] / r;
    matrix[10] = a[2] / r;
    matrix[0] = matrix[10] * b[1] - matrix[9] * b[2];
    matrix[1] = matrix[8] * b[2] - matrix[10] * b[0];
    matrix[2] = matrix[9] * b[0] - matrix[8] * b[1];
    r = math_sqrt(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]);
    matrix[0] = matrix[0] / r;
    matrix[1] = matrix[1] / r;
    matrix[2] = matrix[2] / r;
    matrix[4] = matrix[9] * matrix[2] - matrix[10] * matrix[1];
    matrix[5] = matrix[10] * matrix[0] - matrix[8] * matrix[2];
    matrix[6] = matrix[8] * matrix[1] - matrix[9] * matrix[0];
    r = math_sqrt(matrix[4] * matrix[4] + matrix[5] * matrix[5] + matrix[6] * matrix[6]);
    matrix[4] = matrix[4] / r;
    matrix[5] = matrix[5] / r;
    matrix[6] = matrix[6] / r;
    matrix[3] = 0;
    matrix[7] = 0;
    matrix[11] = 0;
    matrix[15] = 1;
}