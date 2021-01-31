#ifndef __LINALG_H_
#define __LINALG_H_

#include "glad/glad.h"
#include <cstring>
#include <cmath>

struct vec4;

struct vec3 {
    GLfloat arr[3];
    vec3();
    vec3(const GLfloat arr[3]);
    vec3(GLfloat x, GLfloat y, GLfloat z);
    vec3(const vec3& v);
    vec3(const vec4& v);

    void operator+=(const vec3& m);
    vec3 operator+(const vec3& m) const;
    GLfloat operator*(const vec3& m) const; //Dot product
    vec3 operator*(float f) const;

    float norm() const;
    float norm2() const;
    vec3 cross(const vec3& v) const;
};

struct vec4 {
    GLfloat arr[4];
    vec4();
    vec4(const GLfloat arr[4]);
    vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat h);
    vec4(const vec4& v);

    void operator+=(const vec4& m);
    vec4 operator+(const vec4& m) const;
    GLfloat operator*(const vec4& m) const; //Dot product
    vec4 operator*(float f) const;

    float norm() const;
    float norm2() const;

    static vec4 point(const vec3& v)
    {
        vec4 res;
        memcpy(res.arr, v.arr, 3 * sizeof(GLfloat));
        res.arr[3] = 1;
        return res;
    }

    static vec4 vector(const vec3& v)
    {
        vec4 res;
        memcpy(res.arr, v.arr, 3 * sizeof(GLfloat));
        res.arr[3] = 0;
        return res;
    }
};

class NotInvertibleMatrixException {};

struct LU4;

struct mat4 {
    GLfloat arr[16];
    mat4();
    mat4(const GLfloat arr[16]);
    mat4(const mat4& m);

    mat4 transpose() const;
    LU4 LUDecomp() const;
    mat4 inverse() const;
    mat4 inverseAffineIsometry() const;

    void operator+=(const mat4& m);
    mat4 operator+(const mat4& m) const;
    void operator*=(const mat4& m);
    mat4 operator*(const mat4& m) const;
    vec4 operator*(const vec4& v) const;
    vec4 operator^(const vec4& v) const; //Transposed multiplication

    static mat4 Ident()
    {
        mat4 res;
        memset(res.arr, 0, 16 * sizeof(GLfloat));
        res.arr[0] = res.arr[5] = res.arr[10] = res.arr[15] = 1.0;
        return res;
    }

    static mat4 Translation(GLfloat v1, GLfloat v2, GLfloat v3)
    {
        mat4 res = Ident();
        res.arr[12] = v1;
        res.arr[13] = v2;
        res.arr[14] = v3;
        return res;
    }

    static mat4 Translation(const vec3& v)
    {
        return Translation(v.arr[0], v.arr[1], v.arr[2]);
    }

    static mat4 Rotation(vec3 axis, float phi)
    {
        float l, s, c, c1;
        mat4 res = Ident();
        l = 1.0 / axis.norm();
        axis = axis * l;
        s = sin(phi);
        c = cos(phi);
        c1 = 1.0 - c;
        float x = axis.arr[0], y = axis.arr[1], z = axis.arr[2];
        res.arr[0] = x * x * c1 + c;
        res.arr[1] = x * y * c1;
        res.arr[4] = x * y * c1;
        res.arr[5] = y * y * c1 + c;
        res.arr[2]  = x * z * c1;
        res.arr[8] = x * z * c1;
        res.arr[6] = y * z * c1;
        res.arr[9] = y * z * c1;
        res.arr[10] = z * z * c1 + c;
        res.arr[6] += s * x;
        res.arr[2] -= s * y;
        res.arr[1] += s * z;
        res.arr[9] -= s * x;
        res.arr[8] += s * y;
        res.arr[4] -= s * z;
        return res;
    }

    static mat4 Ortho(float left, float right, float bottom, float top, float near, float far)
    {
        mat4 res;
        float rl = right - left;
        float tb = top - bottom;
        float nf = near - far;

        memset(res.arr, 0, 16 * sizeof(GLfloat));
        res.arr[0] = 2.0 / rl;
        res.arr[12] = -(right + left) / rl;
        res.arr[5] = 2.0 / tb;
        res.arr[13] = -(top + bottom) / tb;
        res.arr[10] = 2.0 / nf;
        res.arr[14] = (far + near) / nf;
        res.arr[15] = 1.0;
        return res;
    }

    static mat4 Frumstumf(float left, float right, float bottom, float top, float near, float far)
    {
        mat4 res;
        float nn = near * 2;
        float rl = right - left;
        float tb = top - bottom;
        float nf = near - far;

        memset(res.arr, 0, 16 * sizeof(GLfloat));
        res.arr[0] = nn / rl;
        res.arr[8] = (right + left) / rl;
        res.arr[5] = nn / tb;
        res.arr[9] = (top + bottom) / tb;
        res.arr[10] = (far + near) / nf;
        res.arr[14] = far * nn / nf;
        res.arr[11] = -1.0;
        return res;
    }
};

struct LU4 {
    mat4 lu;
    int p[3];

    LU4();
    LU4(mat4 lu, int p[]);

    vec4 solve(const vec4& v) const;
};

class ZeroRotationAxisException {};

struct rot3 {
    //TODO quaternion
    vec3 axis;
    float angle;

    rot3() {}
    rot3(const vec3& axis, float angle);

    vec3 apply(const vec3& v) const;
    rot3 compose(const rot3& r) const;
    rot3 inverse() const;
    mat4 toMatrix() const;

    rot3 operator*(const rot3& r) const { return compose(r); }
    vec3 operator*(const vec3& v) const { return apply(v); }
};

#endif // __LINALG_H_
