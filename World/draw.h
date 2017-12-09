#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <GLFW/glfw3.h>

#pragma clang diagnostic pop

void draw_triangles(GLuint buffer, int count);

#endif
