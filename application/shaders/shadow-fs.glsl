#version 330 core

in vec3 positionWS;

uniform vec3 lightPos;

void main()
{             
    float lightDistance = length(positionWS - lightPos);
    lightDistance = lightDistance/25.0f; //divide by far plane
    gl_FragDepth = lightDistance;
} 