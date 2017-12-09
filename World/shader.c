#include "shader.h"

GLuint loadShader(GLenum type, char *filename) {
    FILE* fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    long flen = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = malloc((size_t)flen);
    
    char c;
    while ((c = getc(fp)) != EOF) {
        strncat(buffer, &c, 1);
    }
    fclose(fp);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar *const *)&buffer, NULL);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint logSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        char* infoLogBuffer = malloc(logSize);
        fprintf(stderr, "In file \'%s\':\n", filename);
        glGetShaderInfoLog(shader, logSize, &logSize, infoLogBuffer);
        fprintf(stderr, "%s\n", infoLogBuffer);
        free(infoLogBuffer);
        glDeleteShader(shader);
    }
    free(buffer);
    return shader;
}
