#include "kore.h"

float math_sqrt(float number) {
    int i;
    float x, y;
    x = number * 0.5;
    y = number;
    i = * (int *) &y;
    i = 0x5f3759df - (i >> 1);
    y = * (float *) &i;
    y = y * (1.5 - (x * y * y));
    y = y * (1.5 - (x * y * y));
    return number * y;
}

float math_randf(uint32 index) {
    index = (index << 13) ^ index;
    return (((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f) * 0.5f;
}

float math_randnf(uint32 index) {
    index = (index << 13) ^ index;
    return (((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f) - 1.0f;
}

uint math_randi(uint32 index) {
    index = (index << 13) ^ index;
    return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

float math_length2d(float *vec) {
    return math_sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}

float math_length3d(float *vec);
float math_distance2d(float *a, float *b);
float math_distance3d(float *a, float *b);
float math_dot2d(float *a, float *b);
float math_dot3d(float *a, float *b);
void math_cross2d(float *output, float *a, float *b);
void math_cross3d(float *output, float *a, float *b);
void math_normalize2d(float *vec);
void math_normalize3d(float *vec);
void math_normalize4d(float *vec);
void math_normal2d(float *output, float *a, float *b);
void math_normal3d(float *output, float *a, float *b, float *c);
void math_reflect2d(float *output, float *pos, float *normal);
void math_reflect3d(float *output, float *pos, float *normal);
void math_flatten2d(float *output, float *pos, float *normal);
void math_flatten3d(float *output, float *pos, float *normal);
void math_project2d(float *output, float *plane_pos, float *normal, float *pos, float *vector);
void math_project3d(float *output, float *plane_pos, float *normal, float *pos, float *vector);
void math_intersect2d(float *output, float *line_a0, float *line_a1, float *line_b0, float *line_b1);

long math_sqrti(long value);
boolean math_normalize2di(int *point, int fixed_point_multiplyer);
boolean math_normalize3di(int *point, int fixed_point_multiplyer);
void math_intersect2di(int *output, int *line_a0, int *line_a1, int *line_b0, int *line_b1);

void math_matrix_clear(float *matrix);
void math_transform3d(float *output, const float *matrix, const float x, const float y, const float z);
void math_transform_inv3d(float *out, const float *matrix, float x, float y, float z);
void math_transform4d(float *output, const float *matrix, const float x, const float y, const float z, const double w);
void math_matrix_multiply(float *output, const float *a, const float *b);
void math_matrix_reverse4x4(float *output, const float *matrix);
void math_quaternion_to_matrix(float *matrix, float x, float y, float z, float w);
void math_matrix_to_quaternion(float *quaternion, float *matrix);
void math_matrix_to_pos_quaternion_scale(float *matrix, float *pos, float *quaternion, float *scale);
void math_pos_quaternion_scale_to_matrix(float *pos, float *quaternion, float *scale, float *matrix);

void math_matrixxy(float *matrix, const float *origo, const float *point_a, const float *point_b);
void math_matrixxz(float *matrix, const float *origo, const float *point_a, const float *point_b);
void math_matrixyx(float *matrix, const float *origo, const float *point_a, const float *point_b);
void math_matrixyz(float *matrix, const float *origo, const float *point_a, const float *point_b);
void math_matrixzx(float *matrix, const float *origo, const float *point_a, const float *point_b);
void math_matrixzy(float *matrix, const float *origo, const float *point_a, const float *point_b);