#include "camera.h"

camera_t* camera_create()
{
  camera_t* cam = calloc(1, sizeof(camera_t));
  cam->x = 0.0f;
  cam->y = 0.0f;
  cam->z = 0.0f;
  cam->rx = 0.0f;
  cam->ry = 0.0f;
  return cam;
}

void camera_forward(camera_t* cam)
{
  cam->x += cosf(cam->rx - M_PI_2) * cosf(cam->ry);
  cam->y += sinf(cam->ry);
  cam->z += sinf(cam->rx - M_PI_2) * cosf(cam->ry);
}

void camera_backward(camera_t* cam)
{
  cam->x -= cosf(cam->rx - M_PI_2) * cosf(cam->ry);
  cam->y -= sinf(cam->ry);
  cam->z -= sinf(cam->rx - M_PI_2) * cosf(cam->ry);
}

void camera_leftward(camera_t* cam)
{

}

void camera_rightward(camera_t* cam)
{

}
