#ifndef MATH3D_H
#define MATH3D_H

#include <stdint.h>

/*
   Freestanding 3D Coordinate Vector Structure.
   Tracks local position, scale, and velocity offsets with zero dynamic allocations.
*/
struct Vec3 {
    float x;
    float y;
    float z;
};

/*
   Freestanding 4x4 Row-Major Transformation Matrix Structure.
   Maps a flat 16-element array to drive standard 3D graphics projection operations.
   Layout Map Coordinates:
   [ 0   1   2   3  ]
   [ 4   5   6   7  ]
   [ 8   9   10  11 ]
   [ 12  13  14  15 ]
*/
struct Mat4 {
    float m[16];
};

/* Expose our raw mathematical execution operations cleanly to C linkage maps */
extern "C" {
    Vec3 vec3_add(Vec3 a, Vec3 b);
    Vec3 vec3_sub(Vec3 a, Vec3 b);
    float vec3_dot(Vec3 a, Vec3 b);
    Vec3 vec3_cross(Vec3 a, Vec3 b);

    Mat4 mat4_identity();
    Mat4 mat4_multiply(Mat4 a, Mat4 b);
    Vec3 mat4_transform(Mat4 m, Vec3 v);
    Mat4 mat4_translate(float tx, float ty, float tz);
}

#endif
