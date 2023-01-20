#version 330
layout(location = 0) in vec3 position;

uniform mat4  projectionMatrix;
uniform mat4  viewMatrix;
uniform mat4  modelMatrix;

out vec3 positionWS;

void main()
{
    positionWS = (modelMatrix * vec4(position, 1.0)).rgb;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}