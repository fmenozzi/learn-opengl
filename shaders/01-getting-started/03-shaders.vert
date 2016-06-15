#version 150 core

in vec3 position;
in vec3 color;

out vec4 vertexColor;

void main() {
    vertexColor = vec4(color, 1.0);
    gl_Position = vec4(position, 1.0);
}
