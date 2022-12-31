#version 330
in vec2 UV;

uniform sampler2D screenTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D emissionTexture;
uniform sampler2D maskTexture;
uniform sampler2D posTexture;
uniform sampler2D uvTexture;

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
