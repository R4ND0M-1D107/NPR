#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4  projectionMatrix;
uniform mat4  viewMatrix;
uniform mat4  modelMatrix;

out vec2 UV;
out mat3 TBN;
out vec3 positionWS;

mat3 setTBN(vec3 nor, vec3 tan, vec3 bitan)
{
    vec3 normalWS = normalize(vec3(modelMatrix * vec4(nor, 0.0)));
    vec3 tangentWS = normalize(vec3(modelMatrix * vec4(tan, 0.0)));
    vec3 bitangentWS = normalize(vec3(modelMatrix * vec4(bitan, 0.0)));

    TBN = mat3(tangentWS, bitangentWS, normalWS);
    return TBN;
}

void main()
{
    TBN = setTBN(normal, tangent, bitangent);
    UV = uv;
    positionWS = (modelMatrix * vec4(position, 1.0)).xyz;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
