#version 330
in vec4 fragmentPosition;

uniform vec3 lightPos;

void main()
{
    float lightDistance = length(fragmentPosition.rgb - lightPos);
    lightDistance = lightDistance/25.0f; //divide by far plane
    gl_FragDepth = lightDistance;
}