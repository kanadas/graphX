#ifndef __LINALG_H_
#define __LINALG_H_

#include "glad/glad.h"
#include <cstring>

struct vec3 {
    GLfloat arr[3];
    vec3();
    vec3(const GLfloat arr[3]);
    vec3(GLfloat x, GLfloat y, GLfloat z);
    vec3(const vec3& v);

    void operator+=(const vec3& m);
    vec3 operator+(const vec3& m);
    GLfloat operator*(const vec3& m); //Dot product
};

struct vec4 {
    GLfloat arr[4];
    vec4();
    vec4(GLfloat arr[4]);
    vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat h);
    vec4(const vec4& v);

    void operator+=(const vec4& m);
    vec4 operator+(const vec4& m);
    GLfloat operator*(const vec4& m); //Dot product

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

struct mat4 {
    GLfloat arr[16];
    mat4();
    mat4(GLfloat arr[16]);
    mat4(const mat4& m);

    mat4 transpose();

    void operator+=(const mat4& m);
    mat4 operator+(const mat4& m);
    void operator*=(const mat4& m);
    mat4 operator*(const mat4& m);
    vec4 operator*(const vec4& v);
    vec4 operator^(const vec4& v); //Transposed multiplication

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

#endif // __LINALG_H_
