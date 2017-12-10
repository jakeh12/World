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

void mat_multiply(float c[], float a[], float b[])
{
    float r[16];
    int i, n, m;

    for (n = 0; n < 4; n++) { // row
        for (m = 0; m < 4; m++) { // colum
            r[4*n+m] = 0.0f;
            for (i = 0; i < 4; i++) { // calculate dot
                r[4*n+m] += a[4*i+m] * b[4*n+i];
            }
        }
    }
    
    for (i = 0; i < 16; i++)
        c[i] = r[i];
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
    // column 0
    m[0] = 1;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    
    // column 1
    m[4] = 0;
    m[5] = 1;
    m[6] = 0;
    m[7] = 0;
    
    // column 2
    m[8] = 0;
    m[9] = 0;
    m[10] = 1;
    m[11] = 0;
    
    // column 3
    m[12] = x;
    m[13] = y;
    m[14] = z;
    m[15] = 1;
}

void mat_scale(float m[], float x, float y, float z)
{
    // column 0
    m[0] = x;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    
    // column 1
    m[4] = 0;
    m[5] = y;
    m[6] = 0;
    m[7] = 0;
    
    // column 2
    m[8] = 0;
    m[9] = 0;
    m[10] = z;
    m[11] = 0;
    
    // column 3
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

void mat_look_at(float m[], float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z)
{
    float x0, x1, x2, y0, y1, y2, z0, z1, z2, len;
    
    if (eye_x == center_x && eye_y == center_y && eye_z == center_z) {
        mat_identity(m);
        return;
    }
    
    z0 = eye_x - center_x;
    z1 = eye_y - center_y;
    z2 = eye_z - center_z;
    
    len = 1 / sqrtf(z0 * z0 + z1 * z1 + z2 * z2);
    z0 *= len;
    z1 *= len;
    z2 *= len;
    
    x0 = up_y * z2 - up_z * z1;
    x1 = up_z * z0 - up_x * z2;
    x2 = up_x * z1 - up_y * z0;
    len = sqrtf(x0 * x0 + x1 * x1 + x2 * x2);
    if (!len) {
        x0 = 0;
        x1 = 0;
        x2 = 0;
    } else {
        len = 1 / len;
        x0 *= len;
        x1 *= len;
        x2 *= len;
    }
    
    y0 = z1 * x2 - z2 * x1;
    y1 = z2 * x0 - z0 * x2;
    y2 = z0 * x1 - z1 * x0;
    
    len = sqrtf(y0 * y0 + y1 * y1 + y2 * y2);
    if (!len) {
        y0 = 0;
        y1 = 0;
        y2 = 0;
    } else {
        len = 1 / len;
        y0 *= len;
        y1 *= len;
        y2 *= len;
    }
    
    m[0] = x0;
    m[1] = y0;
    m[2] = z0;
    m[3] = 0;
    m[4] = x1;
    m[5] = y1;
    m[6] = z1;
    m[7] = 0;
    m[8] = x2;
    m[9] = y2;
    m[10] = z2;
    m[11] = 0;
    m[12] = -(x0 * eye_x + x1 * eye_y + x2 * eye_z);
    m[13] = -(y0 * eye_x + y1 * eye_y + y2 * eye_z);
    m[14] = -(z0 * eye_x + z1 * eye_y + z2 * eye_z);
    m[15] = 1;
}

void set_matrix(float m[], float x, float y, float z, float rx, float ry)
{
    float a[16];
    float b[16];
    mat_identity(a);
    mat_translate(b, -x, -y, -z);
    mat_multiply(a, b, a);
    mat_rotate(b, cosf(rx), 0.0f, sinf(rx), ry);
    mat_multiply(a, b, a);
    mat_rotate(b, 0.0f, 1.0f, 0.0f, -rx);
    mat_multiply(a, b, a);
    mat_perspective(b, deg_to_rad(120.0f), 4.0f/3.0, 0.1f, 100.0f);
    mat_multiply(a, b, a);
    mat_identity(m);
    mat_multiply(m, a, m);
}

void mat_print(float m[])
{
    printf("[%f %f %f %f;%f %f %f %f;%f %f %f %f;%f %f %f %f]\n", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);

    /*
    printf("----------------------------------------------------------------\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n----------------------------------------------------------------\n", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
*/
    
}
