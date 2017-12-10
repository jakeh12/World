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
    float strafe = atan2f(-1.0f, 0.0f);
    cam->x += cosf(cam->rx + strafe);
    cam->y += 0.0f;
    cam->z += sinf(cam->rx + strafe);
}

void camera_backward(camera_t* cam)
{
    float strafe = atan2f(1.0f, 0.0f);
    cam->x += cosf(cam->rx + strafe);
    cam->y += 0.0f;
    cam->z += sinf(cam->rx + strafe);
}

void camera_leftward(camera_t* cam)
{
    float strafe = atan2f(0.0f, -1.0f);
    cam->x += cosf(cam->rx + strafe);
    cam->y += 0.0f;
    cam->z += sinf(cam->rx + strafe);
}

void camera_rightward(camera_t* cam)
{
    float strafe = atan2f(0.0f, 0.0f);
    cam->x += cosf(cam->rx + strafe);
    cam->y += 0.0f;
    cam->z += sinf(cam->rx + strafe);
}
