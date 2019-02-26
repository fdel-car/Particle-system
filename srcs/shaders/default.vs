#version 410 core
layout (location = 0) in vec4 position;

// uniform mat4 M;
uniform mat4 VP;

out vec3 wPos;

void main()
{
    gl_Position = VP * /* M * */ position;
    wPos = position.xyz;
}
