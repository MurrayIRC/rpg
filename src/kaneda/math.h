#ifndef MATH_H
#define MATH_H

#include "kore.h"
#include "log.h"
#include <assert.h>
#include <math.h>

#define MATH_INLINE static inline

/*------- Math Types -------
Types like vec2 and mat4 which we use in the library.
*/

typedef union vec2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    struct {
        float left, right;
    };
    struct {
        float width, height;
    };
    float elements[2];
} vec2;

typedef union vec3 {
    struct {
        float x, y, z;
    };
    struct {
        float u, v, w;
    };
    struct {
        float r, g, b;
    };
    struct {
        float width, height, depth;
    };
    float elements[3];
} vec3;

typedef union vec4 {
    struct {
        union {
            vec3 xyz;
            struct {
                float x, y, z;
            };
        };
        float w;
    };
    struct {
        union {
            vec3 rgb;
            struct {
                float r, g, b;
            };
        };
        float a;
    };
    float elements[4];
} vec4;

typedef struct mat4 {
    float elements[16];
} mat4;

typedef union quaternion {
    struct {
        union {
            vec3 xyz;
            struct {
                float x, y, z;
            };
        };
        float w;
    };
    float elements[4];
} quaternion;

/* John Carmack's fast sqrt! */
static inline float math_sqrt(float number) {
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

/*------- Random number generators -------
These are very fast pseudo random number generators useful for graphics applications. NOT
cryptographically safe in any way.
*/

/* ranged 0 to 1*/
MATH_INLINE float math_randf(uint32 index) {
    index = (index << 13) ^ index;
    return (((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) /
            1073741824.0f) *
           0.5f;
}

/* ranged -1 to 1*/
MATH_INLINE float math_randnf(uint32 index) {
    index = (index << 13) ^ index;
    return (((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) /
            1073741824.0f) -
           1.0f;
}

/* integer version */
MATH_INLINE uint math_randi(uint32 index) {
    index = (index << 13) ^ index;
    return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

/*------- Normie math -------
This is just math library stuff i need.
*/

MATH_INLINE float math_deg2rad(float degrees) {
    return degrees * (PI / 180.0f);
}

MATH_INLINE float math_rad2deg(float radians) {
    return radians * (180.0f / PI);
}


/*------- Vector math -------
These are common functions used in vector math. */

/* Returns a vec2 filled with the given values. */
MATH_INLINE vec2 math_vec2(const float x, const float y) {
    return (vec2){x, y};
}

/* Returns a vec3 filled with the given values. */
MATH_INLINE vec3 math_vec3(const float x, const float y, const float z) {
    return (vec3){x, y, z};
}

/* Returns a vec4 filled with the given values. */
MATH_INLINE vec4 math_vec4(const float x, const float y, const float z, const float w) {
    return (vec4){x, y, z, w};
}

/* Returns a vec2 filled with 0's. */
MATH_INLINE vec2 math_vec2_zero(void) {
    return (vec2){0};
}

/* Returns a vec3 filled with 0's. */
MATH_INLINE vec3 math_vec3_zero(void) {
    return (vec3){0};
}

/* Returns a vec4 filled with 0's. */
MATH_INLINE vec4 math_vec4_zero(void) {
    return (vec4){0};
}

/* Returns a vec2 of the addition between a and b. */
MATH_INLINE vec2 math_vec2_add(const vec2 a, const vec2 b) {
    return math_vec2(a.x + b.x, a.y + b.y);
}

/* Returns a vec3 of the addition between a and b. */
MATH_INLINE vec3 math_vec3_add(const vec3 a, const vec3 b) {
    return math_vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/* Returns a vec4 of the addition between a and b. */
MATH_INLINE vec4 math_vec4_add(const vec4 a, const vec4 b) {
    return math_vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

/* Returns a vec2 of the difference between a and b. */
MATH_INLINE vec2 math_vec2_subtract(const vec2 a, const vec2 b) {
    return math_vec2(a.x - b.x, a.y - b.y);
}

/* Returns a vec3 of the difference between a and b. */
MATH_INLINE vec3 math_vec3_subtract(const vec3 a, const vec3 b) {
    return math_vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/* Returns a vec4 of the difference between a and b. */
MATH_INLINE vec4 math_vec4_subtract(const vec4 a, const vec4 b) {
    return math_vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

/* Computes the length of a vector 2D for 32 bit floats.*/
MATH_INLINE float math_vec2_length(const vec2 vec) {
    return math_sqrt(vec.x * vec.x + vec.y * vec.y);
}

/* Computes the length of a vector 3D for 32 bit floats.*/
MATH_INLINE float math_vec3_length(const vec3 vec) {
    return math_sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

MATH_INLINE float math_vec4_length(const vec4 vec) {
    return math_sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

/* Computes the distance between two points in 2D for 32 bit floats.*/
MATH_INLINE float math_vec2_distance(const vec2 a, const vec2 b) {
    return math_sqrt(math_vec2_length(math_vec2_subtract(a, b)));
}

/* Computes the distance between two points in 3D for 32 bit floats.*/
MATH_INLINE float math_vec3_distance(const vec3 a, const vec3 b) {
    return math_sqrt(math_vec3_length(math_vec3_subtract(a, b)));
}

/* Computes the dot product between two points in 2D for 32 bit floats.*/
MATH_INLINE float math_vec2_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

/* Computes the dot product between two points in 3D for 32 bit floats.*/
MATH_INLINE float math_vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* Computes cross product between two points in 2D for 32 bit floats.*/
MATH_INLINE vec2 math_vec2_cross(vec2 a, vec2 b) {
    return math_vec2(a.y, -a.x);
}

/* Computes cross product between two points in 3D for 32 bit floats.*/
MATH_INLINE vec3 math_vec3_cross(vec3 a, vec3 b) {
    return math_vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

/* Normalizes a 2D vector of 32 bit floats.*/
MATH_INLINE vec2 math_vec2_normalize(vec2 vec) {
    float f = math_sqrt(math_vec2_length(vec));
    return math_vec2(vec.x / f, vec.y / f);
}

/* Normalizes a 3D vector of 32 bit floats.*/
MATH_INLINE vec3 math_vec3_normalize(vec3 vec) {
    float f = math_sqrt(math_vec3_length(vec));
    return math_vec3(vec.x / f, vec.y / f, vec.z / f);
}

/* Normalizes a 4D vector of 32 bit floats (useful for quaternions).*/
MATH_INLINE vec4 math_vec4_normalize(vec4 vec) {
    float f = math_sqrt(math_vec4_length(vec));
    return math_vec4(vec.x / f, vec.y / f, vec.z / f, vec.w / f);
}

/* Generates a normal from 2 points on a line. */
MATH_INLINE vec2 math_vec2_normal(const vec2 a, const vec2 b) {
    vec2 output = math_vec2(a.y - b.y, b.x - a.x);
    float f = math_sqrt(math_vec2_length(output));
    output.x /= f;
    output.x /= f;
    return output;
}

/* Generates a normal from 3 points on a plane. */
MATH_INLINE vec3 math_vec3_normal(const vec3 a, const vec3 b, const vec3 c) {
    vec3 a2 = math_vec3(a.x - c.x, a.y - c.y, a.z - c.z);
    vec3 b2 = math_vec3(b.x - c.x, b.y - c.y, b.z - c.z);
    vec3 output = math_vec3(
        a2.y * b2.z - a2.z * b2.y, 
        a2.z * b2.x - a2.x * b2.z, 
        a2.x * b2.y - a2.y * b2.x
    );
    float f = math_sqrt(math_vec3_length(output));
    output.x /= f;
    output.y /= f;
    output.z /= f;
    return output;
}

/* Reflects a position to a normal plane in 2D for 32 bit floats.*/
MATH_INLINE void math_reflect2d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1];
    output[0] = pos[0] - normal[0] * 2.0 * f;
    output[1] = pos[1] - normal[1] * 2.0 * f;
}

/* Reflects a position to a normal plane in 3D for 32 bit floats.*/
MATH_INLINE void math_reflect3d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1] + pos[2] * normal[2];
    output[0] = pos[0] - normal[0] * 2.0 * f;
    output[1] = pos[1] - normal[1] * 2.0 * f;
    output[2] = pos[2] - normal[2] * 2.0 * f;
}

/* Flattens a position to a normal plane in 2D for 32 bit floats.*/
MATH_INLINE void math_flatten2d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1];
    output[0] = pos[0] - normal[0] * f;
    output[1] = pos[1] - normal[1] * f;
}

/* Flattens a position to a normal plane in 3D for 32 bit floats.*/
MATH_INLINE void math_flatten3d(float *output, float *pos, float *normal) {
    float f = pos[0] * normal[0] + pos[1] * normal[1] + pos[2] * normal[2];
    output[0] = pos[0] - normal[0] * f;
    output[1] = pos[1] - normal[1] * f;
    output[2] = pos[2] - normal[2] * f;
}

/* projects from aposition along a vector on to a positioned plane in 2D for 32 bit floats.*/
MATH_INLINE void math_project2d(float *output, float *plane_pos, float *normal, float *pos, float *vector) {
    float f = normal[0] * (plane_pos[0] - pos[0]) + normal[1] * (plane_pos[1] - pos[1]);
    f /= normal[0] * vector[0] + normal[1] * vector[1];
    output[0] = pos[0] + vector[0] * f;
    output[1] = pos[1] + vector[1] * f;
}

/* projects fr0m aposition along a vector on to a positioned plane in 3D for 32 bit floats.*/
MATH_INLINE void math_project3d(float *output, float *plane_pos, float *normal, float *pos, float *vector) {
    float f = normal[0] * (plane_pos[0] - pos[0]) + normal[1] * (plane_pos[1] - pos[1]) +
              normal[2] * (plane_pos[2] - pos[2]);
    f /= normal[0] * vector[0] + normal[1] * vector[1] + normal[2] * vector[2];
    output[0] = pos[0] + vector[0] * f;
    output[1] = pos[1] + vector[1] * f;
    output[2] = pos[2] + vector[2] * f;
}

/* Computes the intersection between two lines in 2D for 32 bit floats.*/
MATH_INLINE void math_intersect2d(float *output, float *line_a0, float *line_a1, float *line_b0,
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

/*------- Integer vector math -------------
Vector math for integer types. */

#define sqrt_step(shift)                                                                           \
    if ((0x40000000l >> shift) + root <= value) {                                                  \
        value -= (0x40000000l >> shift) + root;                                                    \
        root = (root >> 1) | (0x40000000l >> shift);                                               \
    } else {                                                                                       \
        root = root >> 1;                                                                          \
    }

/* Integer square root.*/
MATH_INLINE long math_sqrti(long value) {
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

/* Normalizes a 2D vector of integers. The fixed_point_multiplier is used to set what is considered
    to be one. */
MATH_INLINE bool math_normalize2di(int *point, int fixed_point_multiplier) {
    int length = math_sqrti(point[0] * point[0] + point[1] * point[1]);
    if (length != 0) {
        point[0] = (point[0] * fixed_point_multiplier) / length;
        point[1] = (point[1] * fixed_point_multiplier) / length;
        return 1;
    }
    return 0;
}

/* Normalizes a 2D vector of integers. The fixed_point_multiplier is used to set what is considered
    to be one. */
MATH_INLINE bool math_normalize3di(int *point, int fixed_point_multiplier) {
    int length = math_sqrti(point[0] * point[0] + point[1] * point[1] + point[2] * point[2]);
    if (length != 0) {
        point[0] = (point[0] * fixed_point_multiplier) / length;
        point[1] = (point[1] * fixed_point_multiplier) / length;
        point[2] = (point[2] * fixed_point_multiplier) / length;
        return 1;
    }
    return 0;
}

/* Intersects two 2d integer lines. */
MATH_INLINE void math_intersect2di(int *output, int *line_a0, int *line_a1, int *line_b0, int *line_b1) {
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

/*------- Matrix operations ------------------------
Matrix operations for 4x4 matrices.*/

/* Creates an empty matrix (all 0's). */
MATH_INLINE mat4 math_mat4_create(void) {
    return (mat4){0};
}

/* Creates a matrix with a specified left-right diagonal. */
MATH_INLINE mat4 math_mat4_create_with_diagonal(float diagonal) {
    mat4 result = {0};
    result.elements[0] = diagonal;
    result.elements[5] = diagonal;
    result.elements[10] = diagonal;
    result.elements[15] = diagonal;
    return result;
}

/* Creates an identity matrix (a matrix with 1.0f along the left-right diagonal). */
MATH_INLINE mat4 math_mat4_identity(void) {
    return math_mat4_create_with_diagonal(1.0f);
}

/* Multiplies two 4x4 32 bit float matrices.*/
MATH_INLINE mat4 math_mat4_multiply(const mat4 a, const mat4 b) {
    mat4 result = math_mat4_create();
    result.elements[0] = (b.elements[0] * a.elements[0]) + (b.elements[1] * a.elements[4]) + (b.elements[2] * a.elements[8]) + (b.elements[3] * a.elements[12]);
    result.elements[4] = (b.elements[4] * a.elements[0]) + (b.elements[5] * a.elements[4]) + (b.elements[6] * a.elements[8]) + (b.elements[7] * a.elements[12]);
    result.elements[8] = (b.elements[8] * a.elements[0]) + (b.elements[9] * a.elements[4]) + (b.elements[10] * a.elements[8]) + (b.elements[11] * a.elements[12]);
    result.elements[12] = (b.elements[12] * a.elements[0]) + (b.elements[13] * a.elements[4]) + (b.elements[14] * a.elements[8]) + (b.elements[15] * a.elements[12]);

    result.elements[1] = (b.elements[0] * a.elements[1]) + (b.elements[1] * a.elements[5]) + (b.elements[2] * a.elements[9]) + (b.elements[3] * a.elements[13]);
    result.elements[5] = (b.elements[4] * a.elements[1]) + (b.elements[5] * a.elements[5]) + (b.elements[6] * a.elements[9]) + (b.elements[7] * a.elements[13]);
    result.elements[9] = (b.elements[8] * a.elements[1]) + (b.elements[9] * a.elements[5]) + (b.elements[10] * a.elements[9]) + (b.elements[11] * a.elements[13]);
    result.elements[13] = (b.elements[12] * a.elements[1]) + (b.elements[13] * a.elements[5]) + (b.elements[14] * a.elements[9]) + (b.elements[15] * a.elements[13]);

    result.elements[2] = (b.elements[0] * a.elements[2]) + (b.elements[1] * a.elements[6]) + (b.elements[2] * a.elements[10]) + (b.elements[3] * a.elements[14]);
    result.elements[6] = (b.elements[4] * a.elements[2]) + (b.elements[5] * a.elements[6]) + (b.elements[6] * a.elements[10]) + (b.elements[7] * a.elements[14]);
    result.elements[10] = (b.elements[8] * a.elements[2]) + (b.elements[9] * a.elements[6]) + (b.elements[10] * a.elements[10]) + (b.elements[11] * a.elements[14]);
    result.elements[14] = (b.elements[12] * a.elements[2]) + (b.elements[13] * a.elements[6]) + (b.elements[14] * a.elements[10]) + (b.elements[15] * a.elements[14]);

    result.elements[3] = (b.elements[0] * a.elements[3]) + (b.elements[1] * a.elements[7]) + (b.elements[2] * a.elements[11]) + (b.elements[3] * a.elements[15]);
    result.elements[7] = (b.elements[4] * a.elements[3]) + (b.elements[5] * a.elements[7]) + (b.elements[6] * a.elements[11]) + (b.elements[7] * a.elements[15]);
    result.elements[11] = (b.elements[8] * a.elements[3]) + (b.elements[9] * a.elements[7]) + (b.elements[10] * a.elements[11]) + (b.elements[11] * a.elements[15]);
    result.elements[15] = (b.elements[12] * a.elements[3]) + (b.elements[13] * a.elements[7]) + (b.elements[14] * a.elements[11]) + (b.elements[15] * a.elements[15]);

    return result;
}

MATH_INLINE mat4 math_perspective(float fov, float aspect, float z_near, float z_far) {
    assert(aspect != 0);
    assert(z_far != z_near);

    mat4 result = math_mat4_create();

    float q = 1.0f / (float)tan(math_deg2rad(0.5f * fov));
    float a = q / aspect;
    float b = (z_near + z_far) / (z_near - z_far);
    float c = (2.0f * z_near * z_far) / (z_near - z_far);

    result.elements[0 + 0 * 4] = a;
    result.elements[1 + 1 * 4] = q;
    result.elements[2 + 2 * 4] = b;
    result.elements[2 + 3 * 4] = c;
    result.elements[3 + 2 * 4] = -1.0f;

    return result;
}

MATH_INLINE mat4 math_orthographic(float left, float right, float bottom, float top,
                       float z_near, float z_far) {
    mat4 result = math_mat4_identity();

    // Main diagonal
    result.elements[0 + 0 * 4] = 2.0f / (right - left);
    result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
    result.elements[2 + 2 * 4] = -2.0f / (z_far - z_near);

    // Last column
    result.elements[0 + 3 * 4] = -(right + left) / (right - left);
    result.elements[1 + 3 * 4] = -(top + bottom) / (top - bottom);
    result.elements[2 + 3 * 4] = -(z_far + z_near) / (z_far - z_near);

    return result;
}

MATH_INLINE mat4 math_lookat(vec3 pos, vec3 target, vec3 up) {
    vec3 f = math_vec3_normalize(math_vec3_subtract(target, pos));
    vec3 s = math_vec3_normalize(math_vec3_cross(f, up));
    vec3 u = math_vec3_cross(s, f);

    mat4 result = math_mat4_identity();
    result.elements[0 * 4 + 0] = s.x;
    result.elements[1 * 4 + 0] = s.y;
    result.elements[2 * 4 + 0] = s.z;

    result.elements[0 * 4 + 1] = u.x;
    result.elements[1 * 4 + 1] = u.y;
    result.elements[2 * 4 + 1] = u.z;

    result.elements[0 * 4 + 2] = -f.x;
    result.elements[1 * 4 + 2] = -f.y;
    result.elements[2 * 4 + 2] = -f.z;

    result.elements[3 * 4 + 0] = -math_vec3_dot(s, pos);
    result.elements[3 * 4 + 1] = -math_vec3_dot(u, pos);
    result.elements[3 * 4 + 2] = math_vec3_dot(f, pos);

    return result;
}

/*
0, 4, 8,  12
1, 5, 9,  13
2, 6, 10, 14
3, 7, 11, 15
*/
/* Translates a matrix by a 3d point. */
MATH_INLINE mat4 math_translate(mat4 matrix, const vec3 vec) {
    mat4 translate = math_mat4_identity();
    translate.elements[3] = vec.x;
    translate.elements[7] = vec.y;
    translate.elements[11] = vec.z;
    mat4 result = math_mat4_multiply(matrix, translate);
    return result;
}

/* Scales a matrix by a 3d point. */
MATH_INLINE mat4 math_scale(mat4 matrix, const vec3 vec) {
    mat4 scale = math_mat4_identity();
    scale.elements[0] = vec.x;
    scale.elements[5] = vec.y;
    scale.elements[10] = vec.z;
    mat4 result = math_mat4_multiply(matrix, scale);
    return result;
}

/* Rotates a matrix by an angle, assuming a normalized axis. */
MATH_INLINE mat4 math_rotate(mat4 matrix, const float angle, const vec3 axis) {
    float a = angle;
    float c = (float)cos(a);
    float s = (float)sin(a);

    vec3 n_axis = math_vec3_normalize(axis);
    float x = n_axis.x;
    float y = n_axis.y;
    float z = n_axis.z;

    mat4 rotate = math_mat4_identity();
    // First column
    rotate.elements[0 + 0 * 4] = x * x * (1 - c) + c;
    rotate.elements[1 + 0 * 4] = x * y * (1 - c) + z * s;
    rotate.elements[2 + 0 * 4] = x * z * (1 - c) - y * s;

    // Second column
    rotate.elements[0 + 1 * 4] = x * y * (1 - c) - z * s;
    rotate.elements[1 + 1 * 4] = y * y * (1 - c) + c;
    rotate.elements[2 + 1 * 4] = y * z * (1 - c) + x * s;

    // Third column
    rotate.elements[0 + 2 * 4] = x * z * (1 - c) + y * s;
    rotate.elements[1 + 2 * 4] = y * z * (1 - c) - x * s;
    rotate.elements[2 + 2 * 4] = z * z * (1 - c) + c;

    mat4 result = math_mat4_multiply(matrix, rotate);
    return result;
}

/* Transforms a 3D point with a 4x4 32 bit float matrix.*/
MATH_INLINE void math_transform3d(float *output, const float *matrix, const float x, const float y,
                      const float z) {
    output[0] = (matrix[0] * x) + (matrix[4] * y) + (matrix[8] * z) + matrix[12];
    output[1] = (matrix[1] * x) + (matrix[5] * y) + (matrix[9] * z) + matrix[13];
    output[2] = (matrix[2] * x) + (matrix[6] * y) + (matrix[10] * z) + matrix[14];
}

MATH_INLINE void math_transform_inv3d(float *out, const float *matrix, float x, float y, float z) {
    x -= matrix[12];
    y -= matrix[13];
    z -= matrix[14];
    out[0] = (matrix[0] * x) + (matrix[1] * y) + (matrix[2] * z);
    out[1] = (matrix[4] * x) + (matrix[5] * y) + (matrix[6] * z);
    out[2] = (matrix[8] * x) + (matrix[9] * y) + (matrix[10] * z);
}

/* Transforms a 4D point with a 4x4 32 bit float matrix.*/
MATH_INLINE void math_transform4d(float *output, const float *matrix, const float x, const float y,
                      const float z, const double w) {
    output[0] = (matrix[0] * x) + (matrix[4] * y) + (matrix[8] * z) + (matrix[12] * w);
    output[1] = (matrix[1] * x) + (matrix[5] * y) + (matrix[9] * z) + (matrix[13] * w);
    output[2] = (matrix[2] * x) + (matrix[6] * y) + (matrix[10] * z) + (matrix[14] * w);
    output[3] = (matrix[3] * x) + (matrix[7] * y) + (matrix[11] * z) + (matrix[15] * w);
}

/* Reverses a 4x4 32 bit float matrix.*/
MATH_INLINE void math_matrix_reverse4x4(float *output, const float *matrix) {
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

/* Converts a 32 bit float quaternion to a 4x4 32 bit float matrix.*/
MATH_INLINE void math_quaternion_to_matrix(float *matrix, float x, float y, float z, float w) {
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

/* Converts a 4x4 32 bit float matrix to a 32 bit float quaternion.*/
MATH_INLINE void math_matrix_to_quaternion(float *quaternion, float *matrix) {
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

/* Converts a 4x4 32 bit float matrix to a 32 bit float position, scale and quaternion.*/
// MATH_INLINE void math_matrix_to_pos_quaternion_scale(mat4 *matrix, vec3 *pos, quaternion *quat, vec3 *scale) {
//     math_matrix_to_quaternion(quat, matrix);
//     pos.x = matrix[12];
//     pos.y = matrix[13];
//     pos.z = matrix[14];
    
//     scale.x = math_vec3_length(matrix->elements);
//     scale.y = math_vec3_length(&matrix->elements[4]);
//     scale.z = math_vec3_length(&matrix->elements[8]);
// }

/* Converts a 32 bit float position, scale and quaternion to a 4x4 32 bit float matrix.*/
MATH_INLINE void math_pos_quaternion_scale_to_matrix(float *pos, float *quaternion, float *scale,
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


/*------- Matrix Creation ------------------------
These functions let you create a matrix from two points and an optional origin (The origin can be
left as NULL). The first vector dominates and the second will be used to determine rotation around
the first vector*/

/* Lets you create a 32 bit float 4x4 matrix using the X and Y vector */
MATH_INLINE void math_matrixxy(float *matrix, const float *origin, const float *point_a, const float *point_b) {
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

/* Lets you create a 32 bit float 4x4 matrix using the X and Z vector */
MATH_INLINE void math_matrixxz(float *matrix, const float *origin, const float *point_a, const float *point_b) {
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

/* Lets you create a 32 bit float 4x4 matrix using the Y and X vector */
MATH_INLINE void math_matrixyx(float *matrix, const float *origin, const float *point_a, const float *point_b) {
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

/* Lets you create a 32 bit float 4x4 matrix using the Y and Z vector */
MATH_INLINE void math_matrixyz(float *matrix, const float *origin, const float *point_a, const float *point_b) {
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

/* Lets you create a 32 bit float 4x4 matrix using the Z and X vector */
MATH_INLINE void math_matrixzx(float *matrix, const float *origin, const float *point_a, const float *point_b) {
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

/* Lets you create a 32 bit float 4x4 matrix using the Z and Y vector */
MATH_INLINE void math_matrixzy(float *matrix, const float *origin, const float *point_a, const float *point_b) {
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

#endif // MATH_H