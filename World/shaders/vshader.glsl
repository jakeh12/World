#version 410 core

in vec3 vertex_position;
in vec2 vertex_texture_position;
out vec2 fragment_texture_position;

uniform mat4 mvp_matrix;

void main() {
    gl_Position = mvp_matrix * vec4(vertex_position, 1.0);
    fragment_texture_position = vertex_texture_position;
}

