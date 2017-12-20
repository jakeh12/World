#include <OpenGL/gl3.h>
#include <stdio.h>
#include "utils.h"
#include "matrix.h"
#include "draw.h"
#include "camera.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

#define WIDTH 800
#define HEIGHT 600

#define TEXTURE_SIZE 512

/*
 TODO:
 1) png texture loader
 2) obj loader
 3) physics (gravity and collisions)
 4) lighting
 5) text rendering
 */

/* global camera_t struct for handlers */
static camera_t* cam;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        int exclusive_control = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
        if (exclusive_control) {
            /* first escape press, release mouse */
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            /* second escape press, exit */
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        camera_forward(cam);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        camera_backward(cam);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        camera_rightward(cam);
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        camera_leftward(cam);
    }
    /* DEBUGGING ONLY */
    printf("X: %f\tY: %f\tZ: %f\n", cam->x, cam->y, cam->z);
    /* DEBUGGING ONLY */
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int exclusive_control = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
        if (exclusive_control) {
            /* handle regular left click */
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double oldx, oldy;
    float sensitivity = 0.2f;
    double dx = xpos - oldx;
    double dy = ypos - oldy;
    
    int exclusive_control = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    if (exclusive_control) {
        /* handle regular movement */
        cam->rx += deg_to_rad(dx * sensitivity);
        cam->ry += -deg_to_rad(dy * sensitivity);
        if (cam->rx < 0)
            cam->rx += deg_to_rad(360.0f);
        else if (cam->rx >= deg_to_rad(360.0f))
            cam->rx -= deg_to_rad(360.0f);
        if (cam->ry > deg_to_rad(90.0f))
            cam->ry = deg_to_rad(90.0f);
        else if (cam->ry < -deg_to_rad(90.0f))
            cam->ry = -deg_to_rad(90.0f);
        oldx = xpos;
        oldy = ypos;
        /* DEBUGGING ONLY */
        printf("RX: %f\tRY: %f\n", rad_to_deg(cam->rx), rad_to_deg(cam->ry));
        /* DEBUGGING ONLY */
    }
    else {
        oldx = xpos;
        oldy = ypos;
    }
}

int main()
{
    GLFWwindow* window = NULL;
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "World", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwMakeContextCurrent(window);
    
    /* create shaders */
    GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, "shaders/vshader.glsl");
    GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, "shaders/fshader.glsl");
    GLuint program = make_program(vertex_shader, fragment_shader);
    
    /* load textures */
    GLuint texture;
    glGenTextures(1, &texture);
    //glActiveTexture(1);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLubyte* texture_data = malloc(sizeof(GLubyte) * TEXTURE_SIZE * TEXTURE_SIZE * 4);
    load_texture("textures/crate_diffuse.png", &texture_data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(glGetUniformLocation(program, "texture_sampler"), 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    
    /* create transformation matrix */
    static GLfloat mvp_matrix[16];
    
    /* tell GL to only draw onto a pixel if the shape is closer to the viewer */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    /* generate VAO */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    /* generate VBO */
    GLuint vbo = generate_buffer(sizeof(cube_vertices), cube_vertices);
    
    /* other stuff */
    cam = camera_create();
    cam->x = 0.0f;
    cam->y = 0.0f;
    cam->z = 0.0f;
    cam->rx = deg_to_rad(0.0f);
    cam->ry = deg_to_rad(0.0f);
    
    double current_frame_time, last_frame_time = 0.0f, frame_dt;
    
    while(!glfwWindowShouldClose(window)) {
        
        current_frame_time = glfwGetTime();
        frame_dt = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;
        
        glUseProgram(program);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /* draw 10 cubes */
        int i;
        for (i = 0; i < 10; i++) {
            set_matrix(mvp_matrix, cam->x + i*2.0f, cam->y + i*2.0f, cam->z + 10.0f, cam->rx, cam->ry);
            glUniformMatrix4fv(glGetUniformLocation(program, "mvp_matrix"), 1, GL_FALSE, mvp_matrix);
            draw_triangles(vbo, 36);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    /* free all resources */
    glBindVertexArray(0); // <-- TODO: is this a correct way to free vao?
    delete_buffer(vbo);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
