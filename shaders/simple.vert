#version 410 core

layout(location = 0) in vec3 vertex_model_space_pos;

void main() {
    gl_Position.xyz = vertex_model_space_pos;
    gl_Position.w = 1.0;
}