#version 410 core

in vec2 fragment_texture_position;
out vec3 pixel_color;

uniform sampler2D texture_sampler;

void main() {
    pixel_color = texture(texture_sampler, fragment_texture_position).rgb;
}
