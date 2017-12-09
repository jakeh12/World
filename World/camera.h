#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>
#include <math.h>

typedef struct {
  float x;
  float y;
  float z;
  float rx;
  float ry;
} camera_t;

camera_t* camera_create(void);
void camera_forward(camera_t* cam);
void camera_backward(camera_t* cam);
void camera_leftward(camera_t* cam);
void camera_rightward(camera_t* cam);

#endif
