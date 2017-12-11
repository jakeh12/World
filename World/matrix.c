#include "matrix.h"

void normalize(float *x, float *y, float *z)
{
    if (*x == 0.0f && *y == 0.0f && *z == 0.0f) {
        return;
    }
    float d = sqrtf((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
    *x /= d;
    *y /= d;
    *z /= d;
}

void mat_identity(float m[])
{
    m[0] = 1;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = 1;
    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
    m[9] = 0;
    m[10] = 1;
    m[11] = 0;
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}

void mat_multiply(float m[], float a[], float b[])
{
    float r[16];
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[4*i+j] = 0.0f;
            for (k = 0; k < 4; k++) {
                r[4*i+j] += a[4*k+j] * b[4*i+k];
            }
        }
    }
    for (i = 0; i < 16; i++)
        m[i] = r[i];
}

void mat_rotate(float m[], float x, float y, float z, float angle) {
    normalize(&x, &y, &z);
    float s = sinf(angle);
    float c = cosf(angle);
    float k = 1 - c;
    m[0] = k * x * x + c;
    m[1] = k * x * y - z * s;
    m[2] = k * z * x + y * s;
    m[3] = 0;
    m[4] = k * x * y + z * s;
    m[5] = k * y * y + c;
    m[6] = k * y * z - x * s;
    m[7] = 0;
    m[8] = k * z * x - y * s;
    m[9] = k * y * z + x * s;
    m[10] = k * z * z + c;
    m[11] = 0;
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}

void mat_translate(float m[], float x, float y, float z)
{
    m[0] = 1;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = 1;
    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
    m[9] = 0;
    m[10] = 1;
    m[11] = 0;
    m[12] = x;
    m[13] = y;
    m[14] = z;
    m[15] = 1;
}

void mat_scale(float m[], float x, float y, float z)
{
    m[0] = x;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = y;
    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
    m[9] = 0;
    m[10] = z;
    m[11] = 0;
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}

void mat_frustum(float m[], float l, float r, float b, float t, float n, float f)
{
    float rl = (r - l),
    tb = (t - b),
    fn = (f - n);
    m[0] = (n * 2) / rl;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = (n * 2) / tb;
    m[6] = 0;
    m[7] = 0;
    m[8] = (r + l) / rl;
    m[9] = (t + b) / tb;
    m[10] = -(f + n) / fn;
    m[11] = -1;
    m[12] = 0;
    m[13] = 0;
    m[14] = -(f * n * 2) / fn;
    m[15] = 0;
}

void mat_perspective(float m[], float fov, float aspect, float near, float far)
{
    float t = near * tanf(fov / 2.0),
    r = t * aspect;
    mat_frustum(m, -r, r, -t, t, near, far);
}

void mat_orthogonal(float m[], float l, float r, float b, float t, float n, float f) {
    float rl = (r - l),
    tb = (t - b),
    fn = (f - n);
    m[0] = 2 / rl;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = 2 / tb;
    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
    m[9] = 0;
    m[10] = -2 / fn;
    m[11] = 0;
    m[12] = -(l + r) / rl;
    m[13] = -(t + b) / tb;
    m[14] = -(f + n) / fn;
    m[15] = 1;
}

void set_matrix(float m[], float x, float y, float z, float rx, float ry)
{
    // TODO: implement object space rotation
    float a[16];
    float b[16];
    mat_identity(a);
    mat_translate(b, -x, -y, -z);
    mat_multiply(a, b, a);
    mat_rotate(b, cosf(rx), 0.0f, sinf(rx), ry);
    mat_multiply(a, b, a);
    mat_rotate(b, 0.0f, 1.0f, 0.0f, -rx);
    mat_multiply(a, b, a);
    mat_perspective(b, deg_to_rad(45.0f), 4.0f/3.0, 0.1f, 100.0f);
    mat_multiply(a, b, a);
    mat_identity(m);
    mat_multiply(m, a, m);
}

void mat_print(float m[])
{
    printf("----------------------------------------------------------------\n"
           "%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n"
           "----------------------------------------------------------------\n",
           m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13],
           m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);
}
