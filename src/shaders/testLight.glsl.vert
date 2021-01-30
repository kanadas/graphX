#version 420

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_Color;

uniform mat4 ModelTransform, MVPTransform;

out VertexData {
    vec4 color;
    vec3 position;
} Out;

void main()
{
    gl_Position = MVPTransform * in_Position;
    vec4 pos = ModelTransform * in_Position;
    Out.position = pos.xyz / pos.w;
    Out.color = in_Color;
}
