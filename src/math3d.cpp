#include "math3d.h"

/*
   Vector Addition: Multi-axis translation calculator.
*/
extern "C" Vec3 vec3_add(Vec3 a, Vec3 b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

/*
   Vector Subtraction: Direct coordinate delta calculator.
*/
extern "C" Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

/*
   Vector Dot Product: Computes spatial angle weights.
*/
extern "C" float vec3_dot(Vec3 a, Vec3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/*
   Vector Cross Product: Calculates orthogonal perpendicular vectors.
*/
extern "C" Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return {
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    };
}

/*
   Generates a clean Identity Matrix.
   Fills the 16-element flat grid with diagonal 1.0f tracking anchors.
*/
extern "C" Mat4 mat4_identity() {
    Mat4 result = {0};
    result.m[0]  = 1.0f;
    result.m[5]  = 1.0f;
    result.m[10] = 1.0f;
    result.m[15] = 1.0f;
    return result;
}

/*
   Monolithic 4x4 Row-Major Matrix Multiplication Grid.
   Computes nested loop dot-products across row-and-column arrays.
*/
extern "C" Mat4 mat4_multiply(Mat4 a, Mat4 b) {
    Mat4 result = {0};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += a.m[(row * 4) + k] * b.m[(k * 4) + col];
            }
            result.m[(row * 4) + col] = sum;
        }
    }
    return result;
}

/*
   Applies a 4x4 Matrix Transformation matrix directly onto a 3D coordinate Vector.
   Accounts for the implicit W scaling factor coordinate component (Vector W = 1.0f).
*/
extern "C" Vec3 mat4_transform(Mat4 m, Vec3 v) {
    return {
        (m.m[0] * v.x) + (m.m[1] * v.y) + (m.m[2] * v.z) + m.m[3],
        (m.m[4] * v.x) + (m.m[5] * v.y) + (m.m[6] * v.z) + m.m[7],
        (m.m[8] * v.x) + (m.m[9] * v.y) + (m.m[10] * v.z) + m.m[11]
    };
}

/*
   Generates a dedicated Spatial Translation Matrix.
*/
extern "C" Mat4 mat4_translate(float tx, float ty, float tz) {
    Mat4 result = mat4_identity();
    result.m[3]  = tx;
    result.m[7]  = ty;
    result.m[11] = tz;
    return result;
}
