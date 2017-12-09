#include "utils.h"

char* load_file(const char* path)
{
  FILE *file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "fopen %s failed: %d %s\n", path, errno, strerror(errno));
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* data = calloc(length + 1, sizeof(char));
  fread(data, 1, length, file);
  data[length] = '\0';
  fclose(file);
  return data;
}

GLuint make_shader(GLenum type, const char* source)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar *const *)(&source), NULL);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    GLchar* info = calloc(length, sizeof(GLchar));
    glGetShaderInfoLog(shader, length, &length, info);
    fprintf(stderr, "glCompileShader failed:\n%s\n", info);
    free(info);
    glDeleteShader(shader);
    shader = 0;
  }
  return shader;
}

GLuint load_shader(GLenum type, const char* path)
{
  char* data = load_file(path);
  GLuint shader = make_shader(type, data);
  free(data);
  return shader;
}

GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    GLchar *info = calloc(length, sizeof(GLchar));
    glGetProgramInfoLog(program, length, NULL, info);
    fprintf(stderr, "glLinkProgram failed:\n%s\n", info);
    free(info);
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return program;
}

GLuint generate_buffer(GLuint size, GLfloat data[])
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return buffer;
}

void delete_buffer(GLuint buffer)
{
    glDeleteBuffers(1, &buffer);
}
