#version 420

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in int texIndex;

uniform mat4 u_MVP; //Model-Viev Projection

out vec4 v_Color;
out vec2 v_TexCoord;
flat out int v_TexIndex;

void main()
{
    gl_Position = u_MVP * position;
    v_Color = color;
    v_TexCoord = texCoord;
    v_TexIndex = texIndex;
}
