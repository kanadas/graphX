#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <glad/glad.h>

#include "algebra.h"

static void rQuickSort ( void *data, int i, int j,
                         char (*Less)(void *data, int i, int j),
                         void (*Swap)(void *data, int i, int j) )
{
  int p, q;

  while ( i < j ) {
    Swap ( data, i, i+(j-i)/2 );
    for ( p = i, q = i+1;  q <= j;  q++ ) {
      if ( Less ( data, q, i ) )
        Swap ( data, ++p, q );
    }
    Swap ( data, i, p );
    if ( p-i < j-p ) {
      rQuickSort ( data, i, p-1, Less, Swap );
      i = p+1;
    }
    else {
      rQuickSort ( data, p+1, j, Less, Swap );
      j = p-1;
    }
  }
} /*rQuickSort*/

void QuickSort ( void *data, int n,
                 char (*Less)(void *data, int i, int j),
                 void (*Swap)(void *data, int i, int j) )
{
  rQuickSort ( data, 0, n-1, Less, Swap );
} /*QuickSort*/

/* ////////////////////////////////////////////////////////////////////////// */
#define IND4(i,j) ((i)+4*(j))

void M4x4Identf ( GLfloat a[16] )
{
  memset ( a, 0, 16*sizeof(GLfloat) );
  a[0] = a[5] = a[10] = a[15] = 1.0;
} /*M4x4Identf*/

void M4x4Translatef ( GLfloat a[16], float x, float y, float z )
{
  M4x4Identf ( a );
  a[12] = x;  a[13] = y;  a[14] = z;
} /*M4x4Translatef*/

void M4x4Scalef ( GLfloat a[16], float sx, float sy, float sz )
{
  M4x4Identf ( a );
  a[0] = sx;  a[5] = sy;  a[10] = sz;
} /*M4x4Scalef*/

void M4x4RotateXf ( GLfloat a[16], float phi )
{
  M4x4Identf ( a );
  a[5] = a[10] = cos ( phi);  a[9] = -(a[6] = sin ( phi ));
} /*M4x4RotateXf*/

void M4x4RotateYf ( GLfloat a[16], float phi )
{
  M4x4Identf ( a );
  a[0] = a[10] = cos ( phi);  a[2] = -(a[8] = sin ( phi ));
} /*M4x4RotateYf*/

void M4x4RotateZf ( GLfloat a[16], float phi )
{
  M4x4Identf ( a );
  a[0] = a[5] = cos ( phi );  a[4] = -(a[1] = sin ( phi ));
} /*M4x4RotateZf*/

void M4x4RotateVf ( GLfloat a[16], float x, float y, float z, float phi )
{
  float l, s, c, c1;

  M4x4Identf ( a );
  l = 1.0/sqrt ( x*x + y*y + z*z );  x *= l;  y *= l;  z *= l;
  s = sin ( phi );  c = cos ( phi );  c1 = 1.0-c;
  a[0] = x*x*c1 + c;  a[1] = a[4] = x*y*c1;  a[5] = y*y*c1 + c;
  a[2] = a[8] = x*z*c1;  a[6] = a[9] = y*z*c1;  a[10] = z*z*c1 + c;
  a[6] += s*x;  a[2] -= s*y;  a[1] += s*z;
  a[9] -= s*x;  a[8] += s*y;  a[4] -= s*z;
} /*M4x4RotateVf*/

void M4x4MTranslatef ( GLfloat a[16], float x, float y, float z )
{
  GLfloat b[16], c[16];

  M4x4Translatef ( b, x, y, z );
  M4x4Multf ( c, a, b );
  M4x4Copyf ( a, c );
} /*M4x4MTranslatef*/

void M4x4MScalef ( GLfloat a[16], float sx, float sy, float sz )
{
  GLfloat b[16], c[16];

  M4x4Scalef ( b, sx, sy, sz );
  M4x4Multf ( c, a, b );
  M4x4Copyf ( a, c );
} /*M4x4MScalef*/

void M4x4MRotateXf ( GLfloat a[16], float phi )
{
  GLfloat b[16], c[16];

  M4x4RotateXf ( b, phi );
  M4x4Multf ( c, a, b );
  M4x4Copyf ( a, c );
} /*M4x4MRotateXf*/

void M4x4MRotateYf ( GLfloat a[16], float phi )
{
  GLfloat b[16], c[16];

  M4x4RotateYf ( b, phi );
  M4x4Multf ( c, a, b );
  M4x4Copyf ( a, c );
} /*M4x4MRotateYf*/

void M4x4MRotateZf ( GLfloat a[16], float phi )
{
  GLfloat b[16], c[16];

  M4x4RotateZf ( b, phi );
  M4x4Multf ( c, a, b );
  M4x4Copyf ( a, c );
} /*M4x4MRotateZf*/

void M4x4MRotateVf ( GLfloat a[16], float x, float y, float z, float phi )
{
  GLfloat b[16], c[16];

  M4x4RotateVf ( b, x, y, z, phi );
  M4x4Multf ( c, a, b );
  M4x4Copyf ( a, c );
} /*M4x4MRotateVf*/

void M4x4Multf ( GLfloat ab[16], const GLfloat a[16], const GLfloat b[16] )
{
  int i, j, k;

  for ( i = 0; i < 4; i++ )
    for ( j = 0; j < 4; j++ ) {
      ab[IND4(i,j)] = a[IND4(i,0)]*b[IND4(0,j)];
      for ( k = 1; k < 4; k++ )
        ab[IND4(i,j)] += a[IND4(i,k)]*b[IND4(k,j)];
    }
} /*M4x4Multf*/

char M4x4LUDecompf ( GLfloat lu[16], int p[3], const GLfloat a[16] )
{
  int     i, j, k;
  GLfloat d;

  M4x4Copyf ( lu, a );
  memset ( p, 0, 3*sizeof(int) );
  for ( j = 0; j < 3; j++ ) {
    d = fabs ( lu[IND4(j,j)] );  p[j] = j;
    for ( i = j+1; i < 4; i++ )
      if ( fabs ( lu[IND4(i,j)] ) > d )
        { d = fabs ( lu[IND4(i,j)] );  p[j] = i; }
    if ( d == 0.0 )
      return false;
    if ( p[j] != j ) {
      i = p[j];
      for ( k = 0; k < 4; k++ )
        { d = lu[IND4(i,k)]; lu[IND4(i,k)] = lu[IND4(j,k)]; lu[IND4(j,k)] = d; }
    }
    for ( i = j+1; i < 4; i++ ) {
      d = lu[IND4(i,j)] /= lu[IND4(j,j)];
      for ( k = j+1; k < 4; k++ )
        lu[IND4(i,k)] -= d*lu[IND4(j,k)];
    }
  }
  return lu[15] != 0.0;
} /*M4x4LUDecompf*/

void M4x4LUsolvef ( GLfloat aiv[4], const GLfloat lu[16],
                    const int p[3], const GLfloat v[4] )
{
  int     i, j;
  GLfloat d;

  memcpy ( aiv, v, 4*sizeof(GLfloat) );
  for ( i = 0; i < 3; i++ )
    if ( p[i] != i ) { d = aiv[i];  aiv[i] = aiv[p[i]];  aiv[p[i]] = d; }
  for ( i = 1; i < 4; i++ )
    for ( j = 0; j < i; j++ ) aiv[i] -= lu[IND4(i,j)]*aiv[j];
  for ( i = 3; i >= 0; i-- ) {
    for ( j = i+1; j < 4; j++ ) aiv[i] -= lu[IND4(i,j)]*aiv[j];
    aiv[i] /= lu[IND4(i,i)];
  }
} /*M4x4LUsolvef*/

char M4x4Invertf ( GLfloat ai[16], const GLfloat a[16] )
{
  int     i, p[3];
  GLfloat lu[16], e[4];

  if ( !M4x4LUDecompf ( lu, p, a ) )
    return false;
  for ( i = 0; i < 4; i++ ) {
    memset ( e, 0, 4*sizeof(GLfloat) );  e[i] = 1.0;
    M4x4LUsolvef ( &ai[4*i], lu, p, e );
  }
  return true;
} /*M4x4Invertf*/

void M4x4UTLTSolvef ( GLfloat ativ[4], const GLfloat lu[16],
                      const int p[3], const GLfloat v[4] )
{
  int     i, j;
  GLfloat d;

  memcpy ( ativ, v, 4*sizeof(GLfloat) );
  for ( i = 0; i <= 3; i++ ) {
    for ( j = 0; j < i; j++ ) ativ[i] -= lu[IND4(j,i)]*ativ[j];
    ativ[i] /= lu[IND4(i,i)];
  }
  for ( i = 2; i >= 0; i-- )
    for ( j = i+1; j < 4; j++ ) ativ[i] -= lu[IND4(j,i)]*ativ[j];
  for ( i = 2; i >= 0; i-- )
    if ( p[i] != i ) { d = ativ[i];  ativ[i] = ativ[p[i]];  ativ[p[i]] = d; }
} /*M4x4UTLTSolvef*/

char M4x4TInvertf ( GLfloat ati[16], const GLfloat a[16] )
{
  int     i, p[3];
  GLfloat lu[16], e[4];

  if ( !M4x4LUDecompf ( lu, p, a ) )
    return false;
  for ( i = 0; i < 4; i++ ) {
    memset ( e, 0, 4*sizeof(GLfloat) );  e[i] = 1.0;
    M4x4UTLTSolvef ( &ati[4*i], lu, p, e );
  }
  return true;
} /*M4x4TInvertf*/

void M4x4InvertAffineIsometryf ( GLfloat ai[16], const GLfloat a[16] )
{
  int i, j;

  for ( i = 0; i < 3; i++ )
    for ( j = 0; j < 3; j++ ) ai[IND4(i,j)] = a[IND4(j,i)];
  for ( i = 0; i < 3; i++ )
    ai[IND4(i,3)] = -(ai[IND4(i,0)]*a[IND4(0,3)] +
                      ai[IND4(i,1)]*a[IND4(1,3)] +
                      ai[IND4(i,2)]*a[IND4(2,3)]);
  ai[3] = ai[7] = ai[11] = 0.0;  ai[15] = 1.0;
} /*M4x4InvertAffineIsometryf*/

void M4x4Transposef ( GLfloat at[16], const GLfloat a[16] )
{
  int i, j;

  for ( i = 0; i < 4; i++ )
    for ( j = 0; j < 4; j++ ) at[IND4(i,j)] = a[IND4(j,i)];
} /*M4x4Transposef*/

void M4x4MultMVf ( GLfloat av[4], const GLfloat a[16], const GLfloat v[4] )
{
  int i, j;

  for ( i = 0; i < 4; i++ ) {
    av[i] = a[IND4(i,0)]*v[0];
    for ( j = 1; j < 4; j++ ) av[i] += a[IND4(i,j)]*v[j];
  }
} /*M4x4MultMVf*/

void M4x4MultMTVf ( GLfloat av[4], const GLfloat a[16], const GLfloat v[4] )
{
  int i, j;

  for ( i = 0; i < 4; i++ ) {
    av[i] = a[IND4(0,i)]*v[0];
    for ( j = 1; j < 4; j++ ) av[i] += a[IND4(j,i)]*v[j];
  }
} /*M4x4MultMTVf*/

/* ////////////////////////////////////////////////////////////////////////// */
void M4x4Frustumf ( GLfloat a[16], GLfloat ai[16],
                    float left, float right, float bottom,
                    float top, float near, float far )
{
  float rl, tb, nf, nn;

  rl = right-left;  tb = top-bottom;
  nf = near-far;    nn = near+near;
  if ( a ) {
    memset ( a, 0, 16*sizeof(GLfloat) );
    a[0] = nn/rl;           a[8] = (right+left)/rl;
    a[5] = nn/tb;           a[9] = (top+bottom)/tb;
    a[10] = (far+near)/nf;  a[14] = far*nn/nf;
    a[11] = -1.0;
  }
  if ( ai ) {
    memset ( ai, 0, 16*sizeof(GLfloat) );
    ai[0] = rl/nn;  ai[12] = (right+left)/nn;
    ai[5] = tb/nn;  ai[13] = (top+bottom)/nn;
    ai[14] = -1.0;
    ai[11] = nf/(far*nn);
    ai[15] = (far+near)/(far*nn);
  }
} /*M4x4Frustumf*/

void M4x4Orthof ( GLfloat a[16], GLfloat ai[16],
                  float left, float right, float bottom,
                  float top, float near, float far )
{
  float rl, tb, nf;

  rl = right-left;  tb = top-bottom;  nf = near-far;
  if ( a ) {
    memset ( a, 0, 16*sizeof(GLfloat) );
    a[0] = 2.0/rl;    a[12] = -(right+left)/rl;
    a[5] = 2.0/tb;    a[13] = -(top+bottom)/tb;
    a[10] = 2.0/nf;   a[14] = (far+near)/nf;
    a[15] = 1.0;
  }
  if ( ai ) {
    memset ( ai, 0, 16*sizeof(GLfloat) );
    ai[0] = 0.5*rl;    ai[12] = 0.5*(right+left);
    ai[5] = 0.5*tb;    ai[13] = 0.5*(top+bottom);
    ai[10] = 0.5*nf;   ai[14] = 0.5*(far+near);
    ai[15] = 1.0;
  }
} /*M4x4Orthof*/

void M4x4SkewFrustumf ( int w, int h, float aspect, float F, float dist,
                        float xv, float yv, float xp, float yp,
                        float near, float far,
                        float *left, float *right, float *bottom, float *top,
                        const GLfloat vm[16],
                        GLfloat shvm[16], GLfloat eyepos[4],
                        GLfloat pm[16], GLfloat pmi[16] )
{
  float   DF, awh, rl, tb, r, l, t, b, xr, yr;
  GLfloat am[16], v[4];
  int     p[3];

  DF = sqrt ( (double)(36*36 + 24*24) ) / F;
  awh = aspect*(float)w/(float)h;
  tb = DF*near/sqrt ( 1.0 + awh*awh );
  rl = tb*awh;
  xr = xv*near/dist - xp*rl/(float)w;
  l = -(r = 0.5*rl);  l -= xr;  r -= xr;
  yr = yv*near/dist - yp*tb/(float)h;
  b = -(t = 0.5*tb);  b -= yr;  t -= yr;
  if ( top ) *top = t;
  if ( left ) *left = l;
  if ( right ) *right = r;
  if ( bottom ) *bottom = b;
  if ( vm ) {
    M4x4Translatef ( am, -xv, -yv, 0.0 );
    M4x4Multf ( shvm, am, vm );
    if ( eyepos ) {
      M4x4LUDecompf ( am, p, shvm );
      v[0] = v[1] = v[2] = 0.0;  v[3] = 1.0;
      M4x4LUsolvef ( eyepos, am, p, v );
    }
  }
  else
    M4x4Translatef ( shvm, xv, yv, 0.0 );
   M4x4Frustumf ( pm, pmi, l, r, b, t, near, far );
} /*M4x4SkewFrustumf*/

void M4x4LookAtf ( GLfloat a[16], float eye[3], float c[3], float up[3] )
{
  float x[3], y[3], z[3], d;
  int   i;

  V3Subtractf ( z, eye, c );
  V3Normalisef ( z );
  d = V3DotProductf ( z, up );
  for ( i = 0; i < 3; i++ )
    y[i] = up[i] - d*z[i];
  V3Normalisef ( y );
  V3CrossProductf ( x, y, z );
  a[0] = x[0];  a[1] = y[0];  a[2] = z[0];   a[3] = 0.0;
  a[4] = x[1];  a[5] = y[1];  a[6] = z[1];   a[7] = 0.0;
  a[8] = x[2];  a[9] = y[2];  a[10] = z[2];  a[11] = 0.0;
  a[12] = -V3DotProductf ( x, eye );
  a[13] = -V3DotProductf ( y, eye );
  a[14] = -V3DotProductf ( z, eye );
  a[15] = 1.0;
} /*M4x4LookAtf*/

/* ////////////////////////////////////////////////////////////////////////// */
void V4Addf ( float v[4], const float v1[4], const float v2[4] )
{
  v[0] = v1[0]+v2[0];  v[1] = v1[1]+v2[1];
  v[2] = v1[2]+v2[2];  v[3] = v1[3]+v2[3];
} /*V4Addf*/

void V4Subtractf ( float v[4], const float v1[4], const float v2[4] )
{
  v[0] = v1[0]-v2[0];  v[1] = v1[1]-v2[1];
  v[2] = v1[2]-v2[2];  v[3] = v1[3]-v2[3];
} /*V4Subtractf*/

float V4DotProductf ( const float v1[4], const float v2[4] )
{
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] + v1[3]*v2[3];
} /*V4DotProductf*/

void V4CrossProductf ( float v[4], const float v1[4], const float v2[4],
                       const float v3[4] )
{
  float a01, a02, a03, a12, a13, a23;

  a01 = v1[0]*v2[1] - v1[1]*v2[0];  a02 = v1[0]*v2[2] - v1[2]*v2[0];
  a03 = v1[0]*v2[3] - v1[3]*v2[0];  a12 = v1[1]*v2[2] - v1[2]*v2[1];
  a13 = v1[1]*v2[3] - v1[3]*v2[1];  a23 = v1[2]*v2[3] - v1[3]*v2[2];
  v[0] = -a23*v3[1]+a13*v3[2]-a12*v3[3];  v[1] = a23*v3[0]-a03*v3[2]+a02*v3[3];
  v[2] = -a13*v3[0]+a03*v3[1]-a01*v3[3];  v[3] = a12*v3[0]-a02*v3[1]+a01*v3[2];
} /*V4CrossProductf*/

float V4Normalisef ( float v[4] )
{
  float s;

  s = sqrt ( V4DotProductf ( v, v ) );
  if ( s > 0.0 )
    { v[0] /= s;  v[1] /= s;  v[2] /= s;  v[3] /= s; }
  return s;
} /*V4Normalisef*/

void V3Addf ( float v[3], const float v1[3], const float v2[3] )
{
  v[0] = v1[0]+v2[0];  v[1] = v1[1]+v2[1];  v[2] = v1[2]+v2[2];
} /*V3Addf*/

void V3Subtractf ( float v[3], const float v1[3], const float v2[3] )
{
  v[0] = v1[0]-v2[0];  v[1] = v1[1]-v2[1];  v[2] = v1[2]-v2[2];
} /*V3Subtractf*/

float V3DotProductf ( const float v1[3], const float v2[3] )
{
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
} /*V3DotProductf*/

void V3CrossProductf ( float v1xv2[3], const float v1[3], const float v2[3] )
{
  v1xv2[0] = v1[1]*v2[2]-v1[2]*v2[1];
  v1xv2[1] = v1[2]*v2[0]-v1[0]*v2[2];
  v1xv2[2] = v1[0]*v2[1]-v1[1]*v2[0];
} /*V3CrossProductf*/

void V3CompRotationsf ( float v[3], float *phi,
                  const float v2[3], float phi2, const float v1[3], float phi1 )
{
  float s2, c2, s1, c1, s, c, v2v1, v2xv1[3];

  s2 = sin ( 0.5*phi2 );  c2 = cos ( 0.5*phi2 );
  s1 = sin ( 0.5*phi1 );  c1 = cos ( 0.5*phi1 );
  v2v1 = V3DotProductf ( v2, v1 );
  V3CrossProductf ( v2xv1, v2, v1 );
  c = c2*c1 - v2v1*s2*s1;
  v[0] = v2[0]*s2*c1 + v1[0]*s1*c2 + v2xv1[0]*s2*s1;
  v[1] = v2[1]*s2*c1 + v1[1]*s1*c2 + v2xv1[1]*s2*s1;
  v[2] = v2[2]*s2*c1 + v1[2]*s1*c2 + v2xv1[2]*s2*s1;
  s = sqrt ( V3DotProductf ( v, v ) );
  if ( s > 0.0 ) {
    v[0] /= s, v[1] /= s, v[2] /= s;
    *phi = 2.0*atan2 ( s, c );
  }
  else
    v[0] = 1.0, v[1] = v[2] = *phi = 0.0;
} /*V3CompRotationsf*/

float V3Normalisef ( float v[3] )
{
  float s;

  s = sqrt ( V3DotProductf ( v, v ) );
  if ( s > 0.0 )
    { v[0] /= s, v[1] /= s, v[2] /= s; }
  return s;
} /*V3Normalisef*/

/* ////////////////////////////////////////////////////////////////////////// */
void M4x4MultMV3f ( GLfloat av[3], const GLfloat a[16], const GLfloat v[3] )
{
  int i, j;

  for ( i = 0; i < 3; i++ ) {
    av[i] = a[IND4(i,0)]*v[0];
    for ( j = 1; j < 3; j++ ) av[i] += a[IND4(i,j)]*v[j];
  }
} /*M4x4MultMV3f*/

void M4x4MultMP3f ( GLfloat ap[3], const GLfloat a[16], const GLfloat p[3] )
{
  int i, j;

  for ( i = 0; i < 3; i++ ) {
    ap[i] = a[IND4(i,0)]*p[0] + a[IND4(i,3)];
    for ( j = 1; j < 3; j++ ) ap[i] += a[IND4(i,j)]*p[j];
  }
} /*M4x4MultMP3f*/

void V3ReflectPointf ( float r[3],
                       const float p[3], const float nv[3], const float q[3] )
{
  float g;

  r[0] = q[0] - p[0];     r[1] = q[1] - p[1];     r[2] = q[2] - p[2];
  g = 2.0*V3DotProductf ( nv, r )/V3DotProductf ( nv, nv );
  r[0] = q[0] - g*nv[0];  r[1] = q[1] - g*nv[1];  r[2] = q[2] - g*nv[2];
} /*V3ReflectPointf*/

void M4x4RotatePVf ( GLfloat a[16],
                     const float p[3], const float v[3], float phi )
{
  GLfloat b[16], c[16];

  M4x4RotateVf ( b, v[0], v[1], v[2], phi );
  M4x4Translatef ( c, -p[0], -p[1], -p[2] );
  M4x4Multf ( a, b, c );
  a[12] += p[0];  a[13] += p[1];  a[14] += p[2];
} /*M4x4RotatePVf*/

char M4x4RotateP2Vf ( GLfloat a[16], const float p[3],
                      const float v1[3], const float v2[3] )
{
  float   v[3], sa, ca, ang;
  GLfloat b[16], c[16];

  V3CrossProductf ( v, v1, v2 );
  sa = sqrt ( V3DotProductf ( v, v ) );
  if ( sa > 0.0 ) {
    ca = V3DotProductf ( v1, v2 );
    ang = atan2 ( sa, ca );
    M4x4RotateVf ( a, v[0], v[1], v[2], ang );
    if ( p ) {
      M4x4Translatef ( b, -p[0], -p[1], -p[2] );
      M4x4Multf ( c, a, b );
      M4x4Translatef ( b, p[0], p[1], p[2] );
      M4x4Multf ( a, b, c );
    }
    return true;
  }
  else
    return false;
} /*M4x4RotateP2Vf*/
