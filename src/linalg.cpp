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
    return vec3(f * arr[0], f * arr[1], f * arr[2]);
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

LU4 mat4::LUDecomp()
{
    int i, j, k;
    GLfloat d;
    //LU4 res(m, vec3(0,0,0));
    mat4 lu(*this);
    int p[3];
    for (j = 0; j < 3; j++) {
        d = fabs(lu.arr[IND4(j, j)]);
        p[j] = j;
        for (i = j + 1; i < 4; i++)
            if (fabs(lu.arr[IND4(i, j)]) > d) {
                d = fabs(lu.arr[IND4(i, j)]);
                p[j] = i;
            }
        if (d == 0.0) {
            throw NotInvertibleMatrixException();
        }
        if (p[j] != j) {
            i = p[j];
            for (k = 0; k < 4; k++) {
                d = lu.arr[IND4(i, k)];
                lu.arr[IND4(i, k)] = lu.arr[IND4(j, k)];
                lu.arr[IND4(j, k)] = d;
            }
        }
        for (i = j + 1; i < 4; i++) {
            d = lu.arr[IND4(i, j)] /= lu.arr[IND4(j, j)];
            for (k = j + 1; k < 4; k++)
                lu.arr[IND4(i, k)] -= d * lu.arr[IND4(j, k)];
        }
    }
    if(lu.arr[15] == 0.0) {
        throw NotInvertibleMatrixException();
    }
    return LU4(lu, p);
}

mat4 mat4::inverse()
{
    mat4 res;
    LU4 lu = LUDecomp();
    for (int i = 0; i < 4; i++) {
        vec4 e(0, 0, 0, 0);
        e.arr[i] = 1.0;
        vec4 col = lu.solve(e);
        memcpy(res.arr + 4 * i, col.arr, 4 * sizeof(GLfloat));
    }
    return res;
}

mat4 mat4::inverseAffineIsometry()
{
    mat4 res;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            res.arr[IND4(i, j)] = arr[IND4(j, i)];
    for (int i = 0; i < 3; i++)
        res.arr[IND4(i, 3)] = -(res.arr[IND4(i, 0)] * arr[IND4(0, 3)] + res.arr[IND4(i, 1)] * arr[IND4(1, 3)] + res.arr[IND4(i, 2)] * arr[IND4(2, 3)]);
    res.arr[3] = 0.0;
    res.arr[7] = 0.0;
    res.arr[11] = 0.0;
    res.arr[15] = 1.0;
    return res;
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

LU4::LU4(mat4 lu, int p[])
    : lu(lu)
{
    memcpy(this->p, p, 3 * sizeof(int));
}

vec4 LU4::solve(const vec4& v)
{
    GLfloat d;
    vec4 res = v;
    for (int i = 0; i < 3; i++)
        if (p[i] != i) {
            d = res.arr[i];
            res.arr[i] = res.arr[p[i]];
            res.arr[p[i]] = d;
        }
    for (int i = 1; i < 4; i++)
        for (int j = 0; j < i; j++)
            res.arr[i] -= lu.arr[IND4(i, j)] * res.arr[j];
    for (int i = 3; i >= 0; i--) {
        for (int j = i + 1; j < 4; j++)
            res.arr[i] -= lu.arr[IND4(i, j)] * res.arr[j];
        res.arr[i] /= lu.arr[IND4(i, i)];
    }
    return res;
}
