#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <GLFW/glfw3.h>

#pragma clang diagnostic pop

GLuint loadShader(GLuint, char *);

#endif
