#version 420

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
    if (v_TexIndex >= 0) {
        color = texture(u_Textures[v_TexIndex], v_TexCoord);
    } else {
        color = v_Color;
    }
}
