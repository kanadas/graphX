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

vec3::vec3(const vec4& v)
{
    memcpy(arr, v.arr, 3 * sizeof(GLfloat));
}

void vec3::operator+=(const vec3& m)
{
    arr[0] += m.arr[0];
    arr[1] += m.arr[1];
    arr[2] += m.arr[2];
}

vec3 vec3::operator+(const vec3& m) const
{
    vec3 res(arr);
    res += m;
    return res;
}
// Dot product
GLfloat vec3::operator*(const vec3& m) const
{
    return arr[0] * m.arr[0] + arr[1] * m.arr[1] + arr[2] * m.arr[2];
}

vec3 vec3::operator*(float f) const
{
    return vec3(f * arr[0], f * arr[1], f * arr[2]);
}

float vec3::norm() const
{
    return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
}

float vec3::norm2() const
{
    return arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2];
}

vec3 vec3::cross(const vec3& v) const
{
    vec3 res;
    res.arr[0] = arr[1] * v.arr[2] - arr[2] * v.arr[1];
    res.arr[1] = arr[2] * v.arr[0] - arr[0] * v.arr[2];
    res.arr[2] = arr[0] * v.arr[1] - arr[1] * v.arr[0];
    return res;
}

vec4::vec4()
{
}

vec4::vec4(const GLfloat arr[4])
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

vec4 vec4::operator+(const vec4& m) const
{
    vec4 res(arr);
    res += m;
    return res;
}

// Dot product
GLfloat vec4::operator*(const vec4& m) const
{
    return arr[0] * m.arr[0] + arr[1] * m.arr[1] + arr[2] * m.arr[2] + arr[3] * m.arr[3];
}

vec4 vec4::operator*(float f) const
{
    return vec4(f * arr[0], f * arr[1], f * arr[2], f * arr[3]);
}

float vec4::norm() const
{
    return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2] + arr[3] * arr[3]);
}

float vec4::norm2() const
{
    return arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2] + arr[3] * arr[3];
}

#define IND4(i, j) ((i) + 4 * (j))

mat4::mat4()
{
}

mat4::mat4(const GLfloat arr[16])
{
    memcpy(this->arr, arr, 16 * sizeof(GLfloat));
}

mat4::mat4(const mat4& m)
{
    memcpy(arr, m.arr, 16 * sizeof(GLfloat));
}

LU4 mat4::LUDecomp() const
{
    int i, j, k;
    GLfloat d;
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
    if (lu.arr[15] == 0.0) {
        throw NotInvertibleMatrixException();
    }
    return LU4(lu, p);
}

mat4 mat4::inverse() const
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

mat4 mat4::inverseAffineIsometry() const
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

mat4 mat4::transpose() const
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

mat4 mat4::operator+(const mat4& m) const
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

mat4 mat4::operator*(const mat4& m) const
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

vec4 mat4::operator*(const vec4& v) const
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

// Transposed multiplication
vec4 mat4::operator^(const vec4& v) const
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

vec4 LU4::solve(const vec4& v) const
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

rot3::rot3(const vec3& axis, float angle)
    : angle(angle)
{
    float norm = axis.norm();
    if(norm == 0) {
        throw new ZeroRotationAxisException();
    }
    this->axis = axis * (1.0f / norm);
}

vec3 rot3::apply(const vec3& v) const
{
    return vec3(toMatrix() * vec4::point(v));
}

//first this, then r
rot3 rot3::compose(const rot3& r) const
{
    rot3 res;
    float s2, c2, s1, c1, s, c, v2v1;

    s2 = sin(0.5 * r.angle);
    c2 = cos(0.5 * r.angle);
    s1 = sin(0.5 * angle);
    c1 = cos(0.5 * angle);
    v2v1 = r.axis * axis;
    vec3 v2xv1 = r.axis.cross(axis);
    c = c2 * c1 - v2v1 * s2 * s1;
    res.axis.arr[0] = r.axis.arr[0] * s2 * c1 + axis.arr[0] * s1 * c2 + v2xv1.arr[0] * s2 * s1;
    res.axis.arr[1] = r.axis.arr[1] * s2 * c1 + axis.arr[1] * s1 * c2 + v2xv1.arr[1] * s2 * s1;
    res.axis.arr[2] = r.axis.arr[2] * s2 * c1 + axis.arr[2] * s1 * c2 + v2xv1.arr[2] * s2 * s1;
    s = res.axis.norm();
    if (s > 0.0) {
        res.axis = res.axis * (1/s);
        res.angle = 2.0 * atan2(s, c);
    } else {
        res.axis = vec3(1,0,0);
        res.angle = 0;
    }
    return res;
}

rot3 rot3::inverse() const
{
    return rot3(axis, -angle);
}

mat4 rot3::toMatrix() const
{
    return mat4::Rotation(axis, angle);
}
