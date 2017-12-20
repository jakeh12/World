#include "utils.h"

char* load_file(const char* path)
{
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "fopen %s failed: %d %s\n", path, errno, strerror(errno));
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    int length = (int)(ftell(file));
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

void load_texture(const char* path)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "fopen %s failed: %d %s\n", path, errno, strerror(errno));
        exit(1);
    }
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(file);
        fprintf(stderr, "png_create_read_struct failed: %d %s\n", errno, strerror(errno)); // TODO: does libpng produce errno?
        exit(1);
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fclose(file);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fprintf(stderr, "png_create_info_struct failed: %d %s\n", errno, strerror(errno)); // TODO: does libpng produce errno?
        exit(1);
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(file);
    }
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
    
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
    
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    unsigned char* data = calloc(sizeof(unsigned char), row_bytes*height);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    int i;
    for (i = 0; i < height; i++) {
        // invert the order of rows
        memcpy(data + (row_bytes * (height-1-i)), row_pointers[i], row_bytes);
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
