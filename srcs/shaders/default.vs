#version 410 core
layout (location = 0) in vec4 position;
layout (location = 2) in vec4 color;

// uniform mat4 M;
uniform mat4 VP;

out vec4 vColor;

void main()
{
    gl_Position = VP * /* M * */ position;
    vColor = color;
}
