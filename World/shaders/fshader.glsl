#version 410 core

in vec4 color;
out vec4 out_color;

void main() {
  //out_color = vec3(1,1,0);
  out_color = vec4(color);
}
