/* credit for this module lies entirely with Eskil Steenberg. Some of his code has been adapted here
 * for my purposes, with some minor convenience edits. see http://gamepipeline.org/ for more. */

#ifndef KORE_H
#define KORE_H

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
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

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

/*------- Random number generators -------
These are very fast pseudo random number generators useful for graphics applications. NOT
cryptographically safe in any way.
*/

extern float math_randf(uint32 index);  /* ranged 0 to 1*/
extern float math_randnf(uint32 index); /* ranged -1 to 1*/
extern uint math_randi(uint32 index);   /* integer version */

/*------- Normie math -------
This is just math library stuff i need.
*/

extern float math_deg2rad(float degrees);
extern float math_rad2deg(float radians);

/*------- Camera-specific math -------
These are functions that help the camera do shit.
*/

extern void math_perspective(float *out_mat, float fov, float aspect, float z_near, float z_far);
extern void math_orthographic(float *out_mat, float left, float right, float bottom, float top,
                              float z_near, float z_far);
extern void math_lookat(float *out_mat, float *vec_pos, float *vec_target, float *vec_up);

/*------- Vector math -------
These are common functions used in vector math. */

/* Computes the length of a vector 2D for 32 bit floats.*/
extern float math_length2d(float *vec);
/* Computes the length of a vector 3D for 32 bit floats.*/
extern float math_length3d(float *vec);
/* Computes the distance between two points in 2D for 32 bit floats.*/
extern float math_distance2d(float *a, float *b);
/* Computes the distance between two points in 3D for 32 bit floats.*/
extern float math_distance3d(float *a, float *b);
/* Computes the dot product between two points in 2D for 32 bit floats.*/
extern float math_dot2d(float *a, float *b);
/* Computes the dot product between two points in 3D for 32 bit floats.*/
extern float math_dot3d(float *a, float *b);
/* Computes cross product between two points in 2D for 32 bit floats.*/
extern void math_cross2d(float *output, float *a, float *b);
/* Computes cross product between two points in 3D for 32 bit floats.*/
extern void math_cross3d(float *output, float *a, float *b);
/* Normalizes a 2D vector of 32 bit floats.*/
extern void math_normalize2d(float *vec);
/* Normalizes a 3D vector of 32 bit floats.*/
extern void math_normalize3d(float *vec);
/* Normalizes a 4D vector of 32 bit floats (useful for quaternions).*/
extern void math_normalize4d(float *vec);
/* Generates a normal from 2 points on a line. */
extern void math_normal2d(float *output, float *a, float *b);
/* Generates a normal from 3 points on a plane. */
extern void math_normal3d(float *output, float *a, float *b, float *c);
/* Reflects a position to a normal plane in 2D for 32 bit floats.*/
extern void math_reflect2d(float *output, float *pos, float *normal);
/* Reflects a position to a normal plane in 3D for 32 bit floats.*/
extern void math_reflect3d(float *output, float *pos, float *normal);
/* Flattens a position to a normal plane in 2D for 32 bit floats.*/
extern void math_flatten2d(float *output, float *pos, float *normal);
/* Flattens a position to a normal plane in 3D for 32 bit floats.*/
extern void math_flatten3d(float *output, float *pos, float *normal);
/* projects from aposition along a vector on to a positioned plane in 2D for 32 bit floats.*/
extern void math_project2d(float *output, float *plane_pos, float *normal, float *pos,
                           float *vector);
/* projects fr0m aposition along a vector on to a positioned plane in 3D for 32 bit floats.*/
extern void math_project3d(float *output, float *plane_pos, float *normal, float *pos,
                           float *vector);
/* Computes the intersection between two lines in 2D for 32 bit floats.*/
extern void math_intersect2d(float *output, float *line_a0, float *line_a1, float *line_b0,
                             float *line_b1);

/*------- Integer vector math -------------
Vector math for integer types. */

/* Integer square root.*/
extern long math_sqrti(long value);
/* Normalizes a 2D vector of integers. The fixed_point_multiplier is used to set what is considerd
 * to be one. */
extern bool math_normalize2di(int *point, int fixed_point_multiplier);
/* Normalizes a 2D vector of integers. The fixed_point_multiplier is used to set what is considerd
 * to be one. */
extern bool math_normalize3di(int *point, int fixed_point_multiplier);
/* Inter sects two 2d integer lines. */
extern void math_intersect2di(int *output, int *line_a0, int *line_a1, int *line_b0, int *line_b1);

/*------- Matrix operations ------------------------
Matrix operations for 4x4 matrices.*/

/* Clears a 4x4 32 bit float matrix to an identity matrix.*/
extern void math_matrix_identity(float *matrix);
/* Translates a matrix by a 3d point. */
extern void math_translate(float *matrix, const float x, const float y, const float z);
/* Scales a matrix by a 3d point. */
extern void math_scale(float *matrix, const float x, const float y, const float z);
/* Rotates a matrix by an angle, assuming a normalized axis. */
extern void math_rotate(float *matrix, const float angle, const float axis_x, const float axis_y,
                        const float axis_z);
/* Transforms a 3D point with a 4x4 32 bit float matrix.*/
extern void math_transform3d(float *output, const float *matrix, const float x, const float y,
                             const float z);
extern void math_transform_inv3d(float *out, const float *matrix, float x, float y, float z);
/* Transforms a 4D point with a 4x4 32 bit float matrix.*/
extern void math_transform4d(float *output, const float *matrix, const float x, const float y,
                             const float z, const double w);
/* Multiplies two 4x4 32 bit float matrices.*/
extern void math_matrix_multiply(float *output, const float *a, const float *b);
/* Reverses a 4x4 32 bit float matrix.*/
extern void math_matrix_reverse4x4(float *output, const float *matrix);
/* Converts a 32 bit float quaternion to a 4x4 32 bit float matrix.*/
extern void math_quaternion_to_matrix(float *matrix, float x, float y, float z, float w);
/* Converts a 4x4 32 bit float matrix to a 32 bit float quaternion.*/
extern void math_matrix_to_quaternion(float *quaternion, float *matrix);
/* Converts a 4x4 32 bit float matrix to a 32 bit float position, scale and quaternion.*/
extern void math_matrix_to_pos_quaternion_scale(float *matrix, float *pos, float *quaternion,
                                                float *scale);
/* Converts a 32 bit float position, scale and quaternion to a 4x4 32 bit float matrix.*/
extern void math_pos_quaternion_scale_to_matrix(float *pos, float *quaternion, float *scale,
                                                float *matrix);

/*------- Matrix Creation ------------------------
These functions let you create a matrix from two points and an optional origin (The origin can be
left as NULL). The first vector dominates and the second will be used to determine rotation around
the first vector*/

/* Lets you create a 32 bit float 4x4 matrix using the X and Y vector */
extern void math_matrixxy(float *matrix, const float *origin, const float *point_a,
                          const float *point_b);
/* Lets you create a 32 bit float 4x4 matrix using the X and Z vector */
extern void math_matrixxz(float *matrix, const float *origin, const float *point_a,
                          const float *point_b);
/* Lets you create a 32 bit float 4x4 matrix using the Y and X vector */
extern void math_matrixyx(float *matrix, const float *origin, const float *point_a,
                          const float *point_b);
/* Lets you create a 32 bit float 4x4 matrix using the Y and Z vector */
extern void math_matrixyz(float *matrix, const float *origin, const float *point_a,
                          const float *point_b);
/* Lets you create a 32 bit float 4x4 matrix using the Z and X vector */
extern void math_matrixzx(float *matrix, const float *origin, const float *point_a,
                          const float *point_b);
/* Lets you create a 32 bit float 4x4 matrix using the Z and Y vector */
extern void math_matrixzy(float *matrix, const float *origin, const float *point_a,
                          const float *point_b);

#endif