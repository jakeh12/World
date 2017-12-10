#include <OpenGL/gl3.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <GLFW/glfw3.h>

#pragma clang diagnostic pop

#include <stdio.h>
#include "utils.h"
#include "matrix.h"
#include "draw.h"
#include "camera.h"

camera_t* cam;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if(key == GLFW_KEY_W && action == GLFW_PRESS) {
        camera_forward(cam);
    }
    else if(key == GLFW_KEY_S && action == GLFW_PRESS) {
        camera_backward(cam);
    }
    else if(key == GLFW_KEY_D && action == GLFW_PRESS) {
        camera_rightward(cam);
    }
    else if(key == GLFW_KEY_A && action == GLFW_PRESS) {
        camera_leftward(cam);
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double oldx, oldy;
    
    float sensitivity = 0.4f;
    
    double dx = xpos - oldx;
    double dy = ypos - oldy;
    
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
    
    window = glfwCreateWindow(800, 600, "World", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    glfwMakeContextCurrent(window);
    
    /* object to render */
    static GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    
    /* create shaders */
    GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, "shaders/vshader.glsl");
    GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, "shaders/fshader.glsl");
    GLuint program = make_program(vertex_shader, fragment_shader);
    
    /* create transformation matrix */
    static GLfloat model_matrix[16];
    static GLfloat view_matrix[16];
    static GLfloat projection_matrix[16];
    static GLfloat mvp_matrix[16];
    static GLfloat temp[16];

    mat_scale(model_matrix, 1.0f, 1.0f, 1.0f); // scale object
    mat_rotate(temp, 0.0f, 0.0f, 1.0f, deg_to_rad(40.0f)); // rotate object
    mat_multiply(model_matrix, model_matrix, temp);
    mat_translate(temp, -3.0f, 0.0f, -3.0f); // position object
    mat_multiply(model_matrix, model_matrix, temp);

    mat_scale(view_matrix, 1.0f, 1.0f, 1.0f); // scale camera
    mat_rotate(temp, 1.0f, 0.0f, 0.0f, deg_to_rad(-20.0f)); // rotate view
    mat_multiply(view_matrix, view_matrix, temp);
    mat_translate(temp, -3.0f, 0.0f, -3.0f); // position object
    mat_multiply(view_matrix, view_matrix, temp);
    
    mat_perspective(projection_matrix, deg_to_rad(120.0f), 4.0f/3.0, 0.1f, 100.0f); // set projection

    mat_multiply(temp, model_matrix, view_matrix);
    mat_multiply(mvp_matrix, projection_matrix, temp);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    
    /* generate VAO */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    /* generate VBO */
    GLuint vbo = generate_buffer(sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
    
    /* other stuff */
    cam = camera_create();
    cam->x = 4.0f;
    cam->y = 3.0f;
    cam->z = 3.0f;
    cam->rx = deg_to_rad(-45.0f);
    cam->ry = deg_to_rad(-30.0f);
    
    while(!glfwWindowShouldClose(window)) {
        
        // **** RENDER ****
        glUseProgram(program);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        //set_matrix(mvp_matrix, cam->x, cam->y, cam->z, cam->rx, cam->ry);
        
        mat_scale(model_matrix, 1.0f, 1.0f, 1.0f); // scale object
        mat_rotate(temp, 0.0f, 0.0f, 0.0f, deg_to_rad(0.0f)); // rotate object
        mat_multiply(model_matrix, model_matrix, temp);
        mat_translate(temp, 0.0f, 0.0f, 0.0f); // position object
        mat_multiply(model_matrix, model_matrix, temp);
        
        mat_scale(view_matrix, 1.0f, 1.0f, 1.0f); // scale camera
        mat_rotate(temp, 1.0f, 0.0f, 0.0f, deg_to_rad(-30.0f)); // rotate view
        mat_multiply(view_matrix, view_matrix, temp);
        
        mat_rotate(temp, 0.0f, 1.0f, 0.0f, deg_to_rad(10.0f)); // rotate view
        mat_multiply(view_matrix, view_matrix, temp);

        mat_translate(temp, 0.0f, -2.0f, -5.0f); // position view
        mat_multiply(view_matrix, view_matrix, temp);
        
        mat_perspective(projection_matrix, deg_to_rad(60.0f), 4.0f/3.0, 0.1f, 100.0f);
        
        mat_multiply(temp, model_matrix, view_matrix);
        mat_multiply(mvp_matrix, projection_matrix, temp);
        
        glUniformMatrix4fv(glGetUniformLocation(program, "mvp_matrix"), 1, GL_FALSE, mvp_matrix);
        
        draw_triangles(vbo, 36);
        // **** DONE RENDER ****
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /* close GL context and any other GLFW resources */
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
