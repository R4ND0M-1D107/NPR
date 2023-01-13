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

uniform float intensity;
uniform float delta;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 emissionColor;
layout(location = 3) out vec4 maskColor;
layout(location = 4) out vec4 posColor;
layout(location = 5) out vec4 uvColor;

vec3 ACES(vec3 color)
{
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	vec3 applied = (color * (a * color + b)) / (color * (c * color + d) + e);
	vec3 clamped = clamp(applied, vec3(0,0,0), vec3(1,1,1));
	return clamped;
}

void main()
{ 
    vec3 color = ACES(texture(screenTexture, UV).xyz);

    fragmentColor = vec4(color, 1.0);
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
