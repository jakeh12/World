#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdlib.h>
#include "utils.h"

void mat_identity(float m[]);
void mat_multiply(float r[], float a[], float* b);
void mat_rotate(float m[], float x, float y, float z, float angle);
void mat_translate(float m[], float x, float y, float z);
void mat_scale(float m[], float x, float y, float z);
void mat_frustum(float m[], float l, float r, float b, float t, float n, float f);
void mat_perspective(float m[], float fov, float aspect, float near, float far);
void mat_orthogonal(float m[], float l, float r, float b, float t, float n, float f);
void mat_look_at(float m[], float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z);
void mat_print(float m[]);
void set_matrix(float m[], float x, float y, float z, float rx, float ry);

#endif
