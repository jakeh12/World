#version 410 core

in vec3 position;
out vec4 color;

uniform mat4 mvp_matrix;

void main() {
  gl_Position = mvp_matrix * vec4(position, 1.0);

  //gl_Position = vec4(position, 1.0);

  color = vec4(position.x + 0.5, position.y + 0.5, position.z + 0.5, 1.0);
}
