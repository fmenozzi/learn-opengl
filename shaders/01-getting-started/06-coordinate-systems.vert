#version 150 core

in vec3 position;
in vec3 color;
in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    ourColor = color;
    TexCoord = vec2(texCoord.x, 1.0-texCoord.y);

    gl_Position = proj * view * model * vec4(position, 1.0);
}
