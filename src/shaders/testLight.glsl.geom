#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in VertexData {
    vec4 color;
    vec3 position;
} In[];

out VertexDataNormal {
    vec4 color;
    vec3 position;
    vec3 normal;
} Out;

void main()
{
    vec3 normal = normalize(cross(In[1].position - In[0].position, In[2].position - In[0].position));
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        Out.position = In[i].position;
        Out.normal = normal;
        Out.color = In[i].color;
        EmitVertex();
    }
    EndPrimitive();
}
