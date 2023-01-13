#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec2 UV;

layout (location = 0) uniform sampler2D screenTexture;
layout (location = 1) uniform sampler2D normalTexture;
layout (location = 2) uniform sampler2D depthTexture;
layout (location = 3) uniform sampler2D emissionTexture;
layout (location = 4) uniform sampler2D maskTexture;
layout (location = 5) uniform sampler2D posTexture;
layout (location = 6) uniform sampler2D uvTexture;

uniform float width;
uniform float height;

out vec4 fragmentColor;
out vec4 normalColor;
out vec4 emissionColor;
out vec4 maskColor;
out vec4 posColor;
out vec4 uvColor;

void main()
{ 
    fragmentColor = vec4(texture(screenTexture, UV).rgb, 1.0);
}
