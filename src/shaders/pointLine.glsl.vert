#version 420

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * position;
    v_Color = color;
}
