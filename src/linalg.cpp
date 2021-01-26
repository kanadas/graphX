#include "linalg.h"
#include <cstring>

#include <cmath>

vec3::vec3()
{
}

vec3::vec3(const GLfloat arr[3])
{
    memcpy(this->arr, arr, 3 * sizeof(GLfloat));
}

vec3::vec3(GLfloat x, GLfloat y, GLfloat z)
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}

vec3::vec3(const vec3& v)
{
    memcpy(arr, v.arr, 3 * sizeof(GLfloat));
}

void vec3::operator+=(const vec3& m)
{
    arr[0] += m.arr[0];
    arr[1] += m.arr[1];
    arr[2] += m.arr[2];
}

vec3 vec3::operator+(const vec3& m)
{
    vec3 res(arr);
    res += m;
    return res;
}

GLfloat vec3::operator*(const vec3& m) //Dot product
{
    return arr[0] * m.arr[0] + arr[1] * m.arr[1] + arr[2] * m.arr[2];
}

vec3 vec3::operator*(float f)
{
    return vec3(f * arr[0], f * arr[1], f* arr[2]);
}

float vec3::norm()
{
    return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
}

float vec3::norm2()
{
    return arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2];
}

vec4::vec4()
{
}

vec4::vec4(GLfloat arr[4])
{
    memcpy(this->arr, arr, 4 * sizeof(GLfloat));
}

vec4::vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat h)
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
    arr[3] = h;
}

vec4::vec4(const vec4& v)
{
    memcpy(arr, v.arr, 4 * sizeof(GLfloat));
}

void vec4::operator+=(const vec4& m)
{
    arr[0] += m.arr[0];
    arr[1] += m.arr[1];
    arr[2] += m.arr[2];
    arr[3] += m.arr[3];
}

vec4 vec4::operator+(const vec4& m)
{
    vec4 res(arr);
    res += m;
    return res;
}

GLfloat vec4::operator*(const vec4& m) //Dot product
{
    return arr[0] * m.arr[0] + arr[1] * m.arr[1] + arr[2] * m.arr[2] + arr[3] * m.arr[3];
}

vec4 vec4::operator*(float f)
{
    return vec4(f * arr[0], f * arr[1], f * arr[2], f * arr[3]);
}

float vec4::norm()
{
    return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2] + arr[3] * arr[3]);
}

float vec4::norm2()
{
    return arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2] + arr[3] * arr[3];
}

#define IND4(i, j) ((i) + 4 * (j))

mat4::mat4()
{
}

mat4::mat4(GLfloat arr[16])
{
    memcpy(this->arr, arr, 16 * sizeof(GLfloat));
}

mat4::mat4(const mat4& m)
{
    memcpy(arr, m.arr, 16 * sizeof(GLfloat));
}

mat4 mat4::transpose()
{
    mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            res.arr[IND4(i, j)] = arr[IND4(j, i)];
    }
    return res;
}

void mat4::operator+=(const mat4& m)
{
    for (int i = 0; i < 16; i++) {
        arr[i] += m.arr[i];
    }
}

mat4 mat4::operator+(const mat4& m)
{
    mat4 res(arr);
    res += m;
    return res;
}

void mat4::operator*=(const mat4& m)
{
    mat4 res = *this * m;
    memcpy(arr, res.arr, 16 * sizeof(GLfloat));
}

mat4 mat4::operator*(const mat4& m)
{
    mat4 res;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            res.arr[IND4(i, j)] = arr[IND4(i, 0)] * m.arr[IND4(0, j)];
            for (int k = 1; k < 4; k++)
                res.arr[IND4(i, j)] += arr[IND4(i, k)] * m.arr[IND4(k, j)];
        }
    return res;
}

vec4 mat4::operator*(const vec4& v)
{
    vec4 res;
    for (int i = 0; i < 4; i++) {
        res.arr[i] = arr[IND4(i, 0)] * v.arr[0];
        for (int j = 1; j < 4; j++) {
            res.arr[i] += arr[IND4(i, j)] * v.arr[j];
        }
    }
    return res;
}

vec4 mat4::operator^(const vec4& v) //Transposed multiplication
{
    vec4 res;
    for (int i = 0; i < 4; i++) {
        res.arr[i] = arr[IND4(0, i)] * v.arr[0];
        for (int j = 1; j < 4; j++) {
            res.arr[i] += arr[IND4(j, i)] * v.arr[j];
        }
    }
    return res;
}
