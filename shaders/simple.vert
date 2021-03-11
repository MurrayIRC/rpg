#version 410 core

layout(location = 0) in vec3 vertex_model_space_pos;
layout(location = 1) in vec3 vertex_color;

uniform mat4 MVP;

out vec3 fragment_color;

void main() {
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = MVP * vec4(vertex_model_space_pos, 1);

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fragment_color = vertex_color;
}