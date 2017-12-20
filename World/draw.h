#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

/* cube vertices */
static GLfloat cube_vertices[] = {
    // x, y, z,     u, v
    // bottom
    -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,  0.0f, 1.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,  0.0f, 1.0f,
    
    // top
    -1.0f, 1.0f,-1.0f,  0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    // front
    -1.0f,-1.0f, 1.0f,  1.0f, 0.0f,
    1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    // back
    -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,  0.0f, 1.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,  0.0f, 1.0f,
    1.0f, 1.0f,-1.0f,   1.0f, 1.0f,
    // left
    -1.0f,-1.0f, 1.0f,  0.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,  1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,  0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,  1.0f, 0.0f,
    // right
    1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
    1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
};

void draw_triangles(GLuint buffer, int count);

#endif
