#version 420

#define MAX_NLIGHTS 8

layout(location = 0) out vec4 out_Color;

in VertexDataNormal {
    vec4 color;
    vec3 position;
    vec3 normal;
} In;

struct LightParam {
    vec4 ambient;
    vec4 direct;
    vec4 position;
    vec3 attenuation; //coefficients
};

uniform LightBlock {
    int nSources;
    LightParam source[MAX_NLIGHTS];
} light;

uniform vec4 eyepos;

vec3 direction(vec4 to, vec3 from, out float dist)
{
    vec3 res;
    if (to.w != 0) {
        res = to.xyz / to.w - from;
        dist = length(res);
    } else {
        //to is in infinity
        res = to.xyz;
    }
    return normalize(res);
}

float attFactor(vec3 att, float dist)
{
    return 1.0/(((att.z * dist) + att.y)*dist + att.x);
}

void main()
{
    //normal is interpolated, so need to normalize
    vec3 normal = normalize(In.normal);
    float dist;
    vec3 eyeDir = direction(eyepos, In.position, dist);
    float eyeAngle = dot(eyeDir, normal);
    out_Color = vec4(0, 0, 0, 1);

    for (int i = 0; i < light.nSources; i++) {
        out_Color += light.source[i].ambient * In.color;
        vec3 lightDir = direction(light.source[i].position, In.position, dist);
        float lightAngle = dot(lightDir, normal);
        if (eyeAngle * lightAngle > 0) {
            //spectator and light are on the same side
            if (light.source[i].position.w != 0) {
                lightAngle *= attFactor(light.source[i].attenuation, dist);
            }
            out_Color += abs(lightAngle) * light.source[i].direct * In.color;
        }
    }

}
